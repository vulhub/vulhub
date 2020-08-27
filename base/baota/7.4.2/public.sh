#!/bin/bash
PATH=/bin:/sbin:/usr/bin:/usr/sbin:/usr/local/bin:/usr/local/sbin:~/bin
pyenv_bin=/www/server/panel/pyenv/bin
rep_path=${pyenv_bin}:$PATH
if [ -d "$pyenv_bin" ];then
	PATH=$rep_path
fi
export PATH
export LANG=en_US.UTF-8
export LANGUAGE=en_US:en

get_node_url(){
	nodes=(http://dg2.bt.cn http://dg1.bt.cn http://123.129.198.197 http://103.224.251.67 http://125.88.182.172:5880 http://45.76.53.20 http://119.188.210.21:5880 http://120.206.184.160 http://113.107.111.78 http://128.1.164.196 http://183.235.223.101:3389);
	tmp_file1=/dev/shm/net_test1.pl
	tmp_file2=/dev/shm/net_test2.pl
	[ -f "${tmp_file1}" ] && rm -f ${tmp_file1}
	[ -f "${tmp_file2}" ] && rm -f ${tmp_file2}
	touch $tmp_file1
	touch $tmp_file2
	for node in ${nodes[@]};
	do
		NODE_CHECK=$(curl --connect-timeout 3 -m 3 2>/dev/null -w "%{http_code} %{time_total}" ${node}/net_test|xargs)
		RES=$(echo ${NODE_CHECK}|awk '{print $1}')
		NODE_STATUS=$(echo ${NODE_CHECK}|awk '{print $2}')
		TIME_TOTAL=$(echo ${NODE_CHECK}|awk '{print $3 * 1000 - 500 }'|cut -d '.' -f 1)
		if [ "${NODE_STATUS}" == "200" ];then
			if [ $TIME_TOTAL -lt 100 ];then
				if [ $RES -ge 1500 ];then
					echo "$RES $node" >> $tmp_file1
				fi
			else
				if [ $RES -ge 1500 ];then
					echo "$TIME_TOTAL $node" >> $tmp_file2
				fi
			fi

			i=$(($i+1))
			if [ $TIME_TOTAL -lt 100 ];then
				if [ $RES -ge 3000 ];then
					break;
				fi
			fi	
		fi
	done

	NODE_URL=$(cat $tmp_file1|sort -r -g -t " " -k 1|head -n 1|awk '{print $2}')
	if [ -z "$NODE_URL" ];then
		NODE_URL=$(cat $tmp_file2|sort -g -t " " -k 1|head -n 1|awk '{print $2}')
		if [ -z "$NODE_URL" ];then
			NODE_URL='http://download.bt.cn';
		fi
	fi
	rm -f $tmp_file1
	rm -f $tmp_file2
}

GetCpuStat(){
	time1=$(cat /proc/stat |grep 'cpu ')
	sleep 1
	time2=$(cat /proc/stat |grep 'cpu ')
	cpuTime1=$(echo ${time1}|awk '{print $2+$3+$4+$5+$6+$7+$8}')
	cpuTime2=$(echo ${time2}|awk '{print $2+$3+$4+$5+$6+$7+$8}')
	runTime=$((${cpuTime2}-${cpuTime1}))
	idelTime1=$(echo ${time1}|awk '{print $5}')
	idelTime2=$(echo ${time2}|awk '{print $5}')
	idelTime=$((${idelTime2}-${idelTime1}))
	useTime=$(((${runTime}-${idelTime})*3))
	[ ${useTime} -gt ${runTime} ] && cpuBusy="true"
	if [ "${cpuBusy}" == "true" ]; then
		cpuCore=$((${cpuInfo}/2))
	else
		cpuCore=$((${cpuInfo}-1))
	fi
}
GetPackManager(){
	if [ -f "/usr/bin/yum" ] && [ -f "/etc/yum.conf" ]; then
		PM="yum"
	elif [ -f "/usr/bin/apt-get" ] && [ -f "/usr/bin/dpkg" ]; then
		PM="apt-get"		
	fi
}

bt_check(){
	p_path=/www/server/panel/class/panelPlugin.py
	if [ -f $p_path ];then
		is_ext=$(cat $p_path|grep btwaf)
		if [ "$is_ext" != "" ];then
			send_check
		fi
	fi
	
	p_path=/www/server/panel/BTPanel/templates/default/index.html
	if [ -f $p_path ];then
		is_ext=$(cat $p_path|grep fbi)
		if [ "$is_ext" != "" ];then
			send_check
		fi
	fi
}

send_check(){
	chattr -i /etc/init.d/bt
	chmod +x /etc/init.d/bt
	p_path2=/www/server/panel/class/common.py
	p_version=$(cat $p_path2|grep "version = "|awk '{print $3}'|tr -cd [0-9.])
	curl -sS --connect-timeout 3 -m 60 http://www.bt.cn/api/panel/notpro?version=$p_version
	NODE_URL=""
	exit 0;
}
init_check(){
	CRACK_URL=(oss.yuewux.com);
	for url in ${CRACK_URL[@]};
	do
		CRACK_INIT=$(cat /etc/init.d/bt |grep ${url})
		if [ "${CRACK_INIT}" ];then
			rm -rf /www/server/panel/class/*
			chattr +i /www/server/panel/class
			chattr -R +i /www/server/panel
			chattr +i /www 
		fi
	done
}
GetSysInfo(){
	if [ "${PM}" = "yum" ]; then
		SYS_VERSION=$(cat /etc/redhat-release)
	elif [ "${PM}" = "apt-get" ]; then
		SYS_VERSION=$(cat /etc/issue)
	fi
	SYS_INFO=$(uname -msr)
	SYS_BIT=$(getconf LONG_BIT)
	MEM_TOTAL=$(free -m|grep Mem|awk '{print $2}')
	CPU_INFO=$(getconf _NPROCESSORS_ONLN)
	GCC_VER=$(gcc -v 2>&1|grep "gcc version"|awk '{print $3}')
	CMAKE_VER=$(cmake --version|grep version|awk '{print $3}')

	echo -e ${SYS_VERSION}
	echo -e Bit:${SYS_BIT} Mem:${MEM_TOTAL}M Core:${CPU_INFO} gcc:${GCC_VER} cmake:${CMAKE_VER}
	echo -e ${SYS_INFO}
}
cpuInfo=$(getconf _NPROCESSORS_ONLN)
if [ "${cpuInfo}" -ge "4" ];then
	GetCpuStat
else
	cpuCore="1"
fi
GetPackManager

if [ -d "/www/server/phpmyadmin/pma" ];then
	rm -rf /www/server/phpmyadmin/pma
	EN_CHECK=$(cat /www/server/panel/config/config.json |grep English)
	if [ "${EN_CHECK}" ];then
		curl http://download.bt.cn/install/update6_en.sh|bash
	else
		curl http://download.bt.cn/install/update6.sh|bash
	fi
fi

if [ ! $NODE_URL ];then
	EN_CHECK=$(cat /www/server/panel/config/config.json |grep English)
	if [ -z "${EN_CHECK}" ];then
		echo '正在选择下载节点...';
	else
		echo "selecting download node...";
	fi
	get_node_url
	bt_check
fi

