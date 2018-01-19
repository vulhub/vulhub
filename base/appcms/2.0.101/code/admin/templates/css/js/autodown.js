//提交给自动同步数据
function downall(url){
	var start = $.trim($("#start").val());
	var end = $.trim($("#end").val());
	if (start && end) {
		if(isNaN(start) && isNaN(end)) {
			C.alert.tips({"content":"开始和结束页必须是数字!"});
			return false;
		}
		if(parseInt(start) > parseInt(end)) { C.alert.tips({"content":"开始页码必须小于结束页码!"}); return false;}
		url += "&start="+start+"&end="+end;
	} else if(!start && end) {
		if(isNaN(end)) { C.alert.tips({"content":"结束页必须是数字!"}); return false;}
		url += "&end="+end;
	} else if(start && !end) {
		if(isNaN(start)) { C.alert.tips({"content":"开始页必须是数字!"}); return false;}
		url += "&start="+start;
	}
    $("#autodowniframe").attr({"src":url});
}
//提交给自动更新数据
function auto_start(auth){
    C.alert.opacty_close();
    C.alert.opacty({"content":"<div style='margin-top:10px;'>定时更新任务已开始！<br>点击其他页面或者关闭浏览器不影响任务执行</div>"});
    $("#auto_stop").css("display","block");
    $("#auto_start").css("display","none");
    $.get('autodown.php?m=auto_start&auth='+auth,{},function(result){
        if(result.length>0){
            try{
                var ret = $.evalJSON(result);
                if(ret.code=='0'){
                    $("#auto_stop").css("display","block");
                    $("#auto_start").css("display","none");
                }else{
                    $("#auto_stop").css("display","none");
                    $("#auto_start").css("display","block");
                }
                C.alert.opacty_close();
                C.alert.opacty({"content":"<div style='margin-top:10px;'>"+ret.msg+"</div>"});
            }catch(e){
                alert('jsErr01:'+e.message+'\r\n'+result);
            }
        }
    });
}
//停止自动更新数据
function auto_stop(auth){
    $("#auto_stop").css("display","none");
    $("#auto_start").css("display","block");
    $.get('autodown_frame.php?m=auto_stop&auth='+auth,{},function(result){
        try{
            var ret = $.evalJSON(result);
            if(ret.code=='0'){
                $("#auto_stop").css("display","none");
                $("#auto_start").css("display","block");
            }else{
                $("#auto_stop").css("display","block");
                $("#auto_start").css("display","none");
            }
            C.alert.opacty_close();
            C.alert.opacty({"content":"<div style='margin-top:10px;'>"+ret.msg+"</div>"});
        }catch(e){
            alert('jsErr02:'+e.message+'\r\n'+result);
        }
    });
}
//显示错误信息
function show_err(result){
    try{
        var ret=$.evalJSON(result);
        C.alert.opacty_close();
        C.alert.opacty({"content":"<div style='margin-top:10px;'>"+ret.msg+"</div>"});
    }catch(e){alert('jsErr:'+e.message+'\r\n'+result);}
}