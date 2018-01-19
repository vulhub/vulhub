var data={"apps":"","serv_cate":"","total_page":0,"cate_id":0};
//初始化页面
$(document).ready(function(){
    get_cate();//获取分类
});

//获取设置分类
function get_cate(){
    var a=$.getJSON(data_center_url+'/appcate.php?jsoncallback=?',{"auth":auth},function(json){
        try{
            if(json.code==0){
                //auth=json.auth;
                //$.post('collect.php?m=set_auth',{"auth":auth},function(data){});
                data.serv_cate=json.data;
                var html='';
                html+='<a id="cate_-1" href="javascript:void(0);" onclick="collect(1,-1);">全部分类</a>';
                html+='<a id="cate_-2" href="javascript:void(0);" onclick="collect(1,-2)"><font color="red">精品推荐</font></a></br>';
                html+='<a style="color:#888;cursor:default;">安卓软件</a>';
			    var np='';
                for(var i=0;i<data.serv_cate.length;i++){
                    if(data.serv_cate[i].tag!=np && i>0 && data.serv_cate[i].tag!='未分类') html+='<br><a style="color:#888;cursor:default;">安卓游戏</a>';
                    np=data.serv_cate[i].tag;
                    html+='<a  id="cate_'+data.serv_cate[i].id+'" href="javascript:void(0);"  onclick="collect(1,'+data.serv_cate[i].id+')" >'+data.serv_cate[i].name+'</a>';
                };
                $("#showcate").html(html);//分类HTML
                $('#bind_html').html(show_bind_html());//绑定分类HTML
                if(auth!='') collect(1,-1);
            }else{
                C.alert.tips({'content':json.msg});
            }
        }catch(e){
            alert(e.message+json);
        }
    });
}
//输出绑定分类HTML
function show_bind_html(){
    var html='';
    for(var i=0;i<data.serv_cate.length;i++){
        html+='<li style="width:110px; _width:90px; float:left; margin:10px 0px 0 25px; display:inline; text-align:center;padding-top:9px;_padding-top:2px;overflow: hidden;white-space: nowrap;text-overflow: ellipsis; ">';
        html+=data.serv_cate[i].name+'<input type="hidden" class="serv_cate" value="'+data.serv_cate[i].id+'" id="'+data.serv_cate[i].id+'">';
        html+='</li>';
        html+='<li style="width:60px;margin:10px 10px 0 10px; float:left; text-align:center;bordre:1px solid #ccc; display:inline; ">';
        html+='<select class="mycate" id="loca_'+data.serv_cate[i].id+'">';
        html+='<option value="0" >本地分类</option>';
        html+=show_option_local();
        html+='</select>';
        html+='</li>';
    }
    return html;
}
//输出本地分类option
function show_option_local(){
    var html='';
    for(var a in local_cate){
        if(local_cate[a].son==0 && local_cate[a].type==0){
            html+='<option value="'+a+'">'+local_cate[a].cname+'</option>';
        }
    }
    return html;
}

//取数据
function collect(p,cate_id,q){
    if(auth=='') {show_getauth(); return false;}
    //精品推荐
	if(cate_id==-2) $("#order").val('2');
	if(!arguments[0]) p=1;
    if((!arguments[1]&&cate_id!=0)||cate_id=='-2') cate_id='-1';
	
    if(!arguments[2]) q='';
    var pagehtml='';
    var pend; //页码显示个数
    var pstart = 1; //开始页码
    var o=$("#order").val();

    if(cate_id!='-1') $("#cate_"+cate_id).addClass('cateselected');
    var cates=data.serv_cate;
    $.each(cates,function(i,v){
    if(cates[i].id!=cate_id)  $("#cate_"+cates[i].id).removeClass('cateselected');
    })

    $("#applist").html('');
    $('.pagecode').html('');
    $("#applist").html('<div id="loaddiv" style="color:green;width:100%;display:block;margin-top:50px;"><img src="./templates/css/img/loading.gif"/>&nbsp;&nbsp;数据载入中，请稍后...</div>');
	data.cate_id = cate_id;
    $.getJSON(data_center_url+'/applist.php?&jsoncallback=?',{"c":cate_id,"p":p,"o":o,"q":q,"auth":auth},function(jsons){
      //alert($.toJSON(jsons));
       try{
            if(jsons.code==0){
                var json=jsons.data;
                var total_page=json.total_page;
                total_page >= 0 ? total_page : 10; //判断总页数值
                data.apps=json.list;
				data.total_page=total_page;
                if(json.list.length==0){
                    var html='<tr><td class="alignleft"><font color=red>没有查询到符合条件的数据</font></td></tr>';
                }else{
                    var html ='<tr>';
                        html +='<th width="80"><a href="javascript:void(0);" onclick="C.form.check_all(\'.collects\');">全选/反选</a></th>';
                        html +='<th width="60">Logo</th>';
                        html +='<th width="70" class="alignleft">应用ID</th>';
                        html +='<th class="alignleft">应用名称</th>';
                        html +='<th>最新版本</th>';
                        html +='<th>类型</th>';
                        html +='<th>大小</th>';
                        html +='<th>评分</th>';
                        html +='<th>下载次数</th>';
                        html +='<th>更新时间</th>';
						html +='<th width="100">添加数据</th>';
                        html +='</tr>';

                    for(var i=0;i<json.list.length;i++){
                        var desc=json.list[i].app_desc
                        desc=desc.replace(/<.*?>/g,'');
                        title=json.list[i].app_title;
                        title=title.replace(/<.*?>/g,'');
                        var tips='title="名称&nbsp;：&nbsp;'+title+'\r\n分类&nbsp;：&nbsp;'+json.list[i].app_cate_name.name+'\r\n简介&nbsp;：&nbsp;'+desc+'"';
                        html +='<tr>';
                        html +='<td><input type="checkbox" class="collects" value="'+json.list[i].app_id+'" /></td>';
                        html +='<td><img src="'+json.list[i].app_logo+'" width=30 height=30/></td>';
                        html +='<td class="alignleft">'+json.list[i].app_id+'</td>';
                        html +='<td class="alignleft" '+tips+'>'+title+'</td>';
                        html +='<td>'+json.list[i].app_version+'</td>';
                        html +='<td>'+json.list[i].app_type+'</td>';
                        html +='<td>'+json.list[i].app_size+'</td>';
                        html +='<td>'+json.list[i].app_recomment+'</td>';
                        html +='<td>'+json.list[i].app_down+'</td>';
                        html +='<td>'+C.date.localtime(json.list[i].app_update_time)+'</td>';
						html +='<td><a href="javascript:void(0);" class="but2 but2s" onclick="zhineng(\'\','+json.list[i].app_id+');">智能</a><a href="javascript:void(0);" class="but2 but2s" onclick="show_cate('+json.list[i].app_id+');">手动</a></td>';
                        html +='</tr>';
                    };
						html +='<tr>';
                        html +='<th width="80"><a href="javascript:void(0);" onclick="C.form.check_all(\'.collects\');">全选/反选</a></th>';
                        html +='<th width="60">Logo</th>';
                        html +='<th width="70" class="alignleft">应用ID</th>';
                        html +='<th class="alignleft">应用名称</th>';
                        html +='<th>最新版本</th>';
                        html +='<th>类型</th>';
                        html +='<th>大小</th>';
                        html +='<th>评分</th>';
                        html +='<th>下载次数</th>';
                        html +='<th>更新时间</th>';
						html +='<th width="100">添加数据</th>';
                        html +='</tr>';
                }
                $("#loaddiv").css({"display":"none"});
                $("#applist").html(html);

                //重新计算right_body 高度
                var rHeight = $("#applist").height() > $(window).height() ?  $("#applist").height() + 350 : $(window).height();//alert(rHeight);
                $('.right_body').css({'height': rHeight });

                p = p > 0 ? p : ($('.pagecode').find('span').text() == "" ? 1 : $('.pagecode').find('span').text()); //获取当前页码
                // 判断循环起止数字
                if(total_page <= 9)
                {
                    pstart = 1;
                    pend = total_page;
                }else{
                    if(total_page <= 4){
                        pstart = 1;
                        pend = 9;
                    }else{
                        pstart = p -3;
                        if (pstart <= 0) pstart = p-1;
                        if (pstart <= 0) pstart = p;
                        pend = pstart + 9;
                        if (pend > total_page) pend = total_page;
                    }
                }

                //中间分页码输出
                if(json.list.length>0){
                    //第一页导航
                    if(pstart > 1) {
                        pagehtml += '<a href="javascript:void(0)" onclick=" collect(1,'+cate_id+',\''+q+'\')" id="pag'+i+'" class="pagehtml">1...</a> ';
                    }

                    if(total_page == 0) p =0; //判断是否有数据
                    for(var i = pstart;i <= pend;i++){
                         i==p ?
                            pagehtml+='<span  id="pag'+i+'" class="npage" >'+i+'</span>' :
                            pagehtml+='<a href="javascript:void(0);" onclick=" collect('+i+','+cate_id+',\''+q+'\')" id="pag'+i+'" class="pagehtml" >'+i+'</a>';
                    }
					
					
					
					
                     //最末页
                    if(p < total_page && pend < total_page) pagehtml += '<a href="javascript:void(0);" onclick=" collect('+total_page+','+cate_id+',\''+q+'\')" id="pag'+total_page+'" class="pagehtml">'+ total_page +'</a>';
                    
					//input表单传值分页
					
					pagehtml+= '<input type="text" class="ipt l" style="width:40px;margin-right:5px;" onkeydown="javascript:if(event.keyCode==13){var page = this.value > data.total_page ? data.total_page : this.value; collect(page,data.cate_id); }"  />';

					
					pagehtml+='<span>共'+json.total+'条 当前'+p+'/'+total_page+'页</span>';
                    //pagehtml+='<span>Query '+json.sql_time+' Seconds</span>';
                    $('.pagecode').html(pagehtml);
                }else{
                    $('.pagecode').html('');
                }
            }else{
                C.alert.tips({'content':jsons.msg});
                $('#loaddiv').css({'color':'red'}).html('数据读取失败：'+jsons.msg);
            }
       }catch(e){
           alert(e.message+jsons);
       }
    });

}


//数据入库
//cateid 是在手动添加应用的时候用的
function into_app(cateid,app_id){
   if(auth=='') {show_getauth(); return false;}
    var is_holder=0;
    if(arguments[0]&&cateid!='') is_holder=1;
    var appids=[];
    $(".collects").each(function(){
    if($(this).attr("checked")=="checked")
        appids.push($(this).val());
    });
	//单个应用添加
	if(arguments[1]&&app_id!='') appids=[app_id];
    var lists=[];
    var dd=data.apps;
    for(var t=0;t<appids.length;t++){
        for(var u=0;u<dd.length;u++){
            if(parseInt(appids[t])==parseInt(dd[u].app_id)){
                var desc=dd[u].app_desc;
                var title=dd[u].app_title;
                desc=desc.replace(/\"/g,"&#34;");
                dd[u].app_desc=desc.replace(/[\]\[},]/g,'');
                title=title.replace(/\"/g,"&#34;");
                dd[u].app_title=title.replace(/[\]\[},]/g,'');
                //if(re!=null||rt!=null) continue;
                lists.push(dd[u]);break;
            }
        }
    }
    if(lists.length==0){C.alert.tips({'content':'请选择入库数据'});return false;}
    var datas=$.toJSON(lists);
    if(is_holder==1){
        $.post('collect.php?m=into_app',{"apps":datas,"cateid":cateid},function(data){
            try{
                C.alert.opacty_close("#show_cate");
                $("#showmessage").html(data);
                C.alert.opacty({"width":"520","height":"270","div_tag":"#message","title":"添加应用信息"});
            }catch(e){alert(data);}
        });
    }else{
        $.post('collect.php?m=into_app',{"apps":datas},function(data){
            try{
                $("#showmessage").html(data);
                C.alert.opacty({"width":"520","height":"270","div_tag":"#message","title":"添加应用信息"});
            }catch(e){alert(data);}
        });
    }
}

//智能添加
function zhineng(cate_id,app_id){
    //判断是否绑定了分类
    $.post('collect.php?m=exsits_bind',function(jsons){
        try{

            var json=eval('('+jsons+')');
            if(json.code==1) {
                //C.alert.tips({'content':"请绑定分类！"});
                blindcate();
                return false;
            }else{
                into_app(cate_id,app_id);

                return true;
                }

        }catch(e){

                alert(e.message+jsons);
                }

        })
    }



//show 绑定 div
function blindcate(){
    selcate();
    C.alert.opacty({"width":"540","height":"400","div_tag":"#blindcate","title":"绑定分类<span style=\"font-weight:normal;color:red;\">（智能添加应用，需要先绑定分类）</span>"});
}
//手动添加 div show_cate
function show_cate(app_id){
    if(auth=='') {show_getauth(); return false;}
	$("#appid").val(app_id);
    C.alert.opacty({"width":"300","height":"85","div_tag":"#show_cate","title":"选择入库分类"});
}

//show 授权码
function show_getauth(){
    C.alert.opacty({"width":"360","height":"220","div_tag":"#getauth","title":"获取授权码"});
}
//获取授权码
function getauth(){
    var qq=$("#qq").val();
	var uid=$("#uid").val();
    $.post('collect.php?m=creatauthfile',function(data){});
    $.getJSON(data_center_url+'/auth.php?jsoncallback=?',{"qq":qq,"uid":uid},function(json){
        try{
            if(json.code==0){
                auth=json.auth;
                $.post('collect.php?m=set_auth',{"auth":auth},function(data){
                   var json=$.evalJSON(data);
                    C.alert.tips({'content':json.msg});
                    if(json.code==0){ setTimeout(function(){ window.location.reload();},1000)}
               });
            }
            C.alert.tips({'content':json.msg});
            }catch(e){

            alert(e.message)}
    });



}

//下拉分类
function selcate(){
    var jsons=data.serv_cate
    var serv_cate=$(".serv_cate");
    $.post('collect.php?m=editbind',function(json){
        try{
            var blindcate=eval('('+json+')');
            for(var t=0;t<blindcate.length;t++){
                var serv = blindcate[t][1];
                var sed = $("#"+blindcate[t][1]).val();
                var selects = $(".mycate");
                if(serv == sed){
                    $("#loca_"+blindcate[t][1]).val(blindcate[t][0]);
                }
           }
        }catch(e){
          C.alert.tips({'content':json});
        }
    });
    var html='';
    for(var i=0;i<jsons.length;i++){
        html+='<option value='+jsons[i].id+'>'+jsons[i].name+'</option>';
    };
    //$(".mycate").append(html);

}

//绑定分类
function saveblind(){
    var my_serv=[];
    var tem=[];
    var serv_cate=$(".serv_cate");
    var my_cate=$(".mycate");
    for(var i=0;i<serv_cate.length;i++){
        tem[0]=my_cate[i].value;
        tem[1]=serv_cate[i].value;
        my_serv.push(tem);
        tem=[];
    }
    var arr=$.toJSON(my_serv);
    $.post('collect.php?m=blindcate',{"my_serv":arr},function(json){
       try{
            json=eval('('+json+')');
            C.alert.tips({"content":""+json.msg+""});
            if(json.code==0){ setTimeout(function(){ window.location.reload();},1000)}
       }catch(e){
            alert(e.message+json);
       }
    });
}