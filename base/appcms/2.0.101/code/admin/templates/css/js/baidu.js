//数据入库
//cateid 是在手动添加应用的时候用的
function submit_app(type){
   var ids=$("#app_ids").val();
    if(type==1){
        $.post("baidu1.php?m=submit",{"app_ids":ids},function(result){
            try {
            var ret = $.evalJSON(result);
			var msg=""
			if(ret.error!=""||ret.message!="")
			msg="成功推送的url条数:"+ret.success+";当天剩余的可推送url条数:"+ret.remain;
			else
			msg="提交错误:"+ret.error+"|"+ret.message;
            alert(msg);
        } catch (e) {
            alert(result + e.message);
        }
        });
    }else{
        $.post("baidu2.php?m=submit",{"app_ids":ids},function(result){
            try {
            var ret = $.evalJSON(result);
			var msg=""
			if(ret.error!=""||ret.message!="")
			msg="成功推送的url条数:"+ret.success+";当天剩余的可推送url条数:"+ret.remain;
			else
			msg="提交错误:"+ret.error+"|"+ret.message;
            alert(msg);
        } catch (e) {
            alert(result + e.message);
        }
        });
    }
}