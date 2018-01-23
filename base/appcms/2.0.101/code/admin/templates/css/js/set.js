$(function(){
    //选项卡
    C.tabs(
        {"params":
        [
            {"nav":"#bn1","con":"#tab1","sclass":"current","nclass":""},
            {"nav":"#bn2","con":"#tab2","sclass":"current","nclass":""},
            {"nav":"#bn3","con":"#tab3","sclass":"current","nclass":""},
            {"nav":"#bn4","con":"#tab4","sclass":"current","nclass":""},
            {"nav":"#bn5","con":"#tab5","sclass":"current","nclass":""}
        ]
    });

    /*最原始的模板*/
    var temp = $('#template').val();
    $('#template').change(function(){
        C.alert.opacty({'width':'300','height':'75','title':'模板更换安全提示','content':'<div style="margin-top:10px;">正在检测模板文件安全隐患，请稍候...</div>'});
        $('.tb3').css({'width':'100%'});
        var file = $('#template').val();
        $.get('safe_check.php?m=safe_check&ajax=json',{'file':file},function(data){//alert(data);
            C.alert.opacty_close('');
            try{
                var ret = $.evalJSON(data);
                if(ret.code == '0'){

                }else{
                    if(!confirm(ret.msg+'\r\n\r\n“确定”继续选择该模板，“取消”选择原模板')){
                        $('#template').val(temp);
                    }
                }
            }catch(e){
                alert(e.message );
            }
        });
    });
	
	
	/*最原始的模板*/
    var temp = $('#wap_tpl').val();
    $('#wap_tpl').change(function(){
        C.alert.opacty({'width':'300','height':'75','title':'模板更换安全提示','content':'<div style="margin-top:10px;">正在检测模板文件安全隐患，请稍候...</div>'});
        $('.tb3').css({'width':'100%'});
        var file = $('#wap_tpl').val();
        $.get('safe_check.php?m=safe_check&ajax=json',{'file':file},function(data){//alert(data);
            C.alert.opacty_close('');
            try{
                var ret = $.evalJSON(data);
                if(ret.code == '0'){

                }else{
                    if(!confirm(ret.msg+'\r\n\r\n“确定”继续选择该模板，“取消”选择原模板')){
                        $('#wap_tpl').val(temp);
                    }
                }
            }catch(e){
                alert(e.message );
            }
        });
    });
});

//保存配置
function save(){
    var sets=C.form.get_form('set');
    $.post('set.php?m=save',sets,function(json){
        try{
            var json=eval('('+json+')');
            C.alert.tips({'content':json.msg});
        }catch(e){alert(json);}
    });
}

//上传图片回调
function callback_upload(ret){
    try{
        //alert(ret);
        var json=$.evalJSON(ret);
        if(json.files.length<=0) {
            alert('上传失败');
            return false;
        }
        $("#site_logo").val(json.files[0].original);
        $("#showcimg").html('<img src="'+json.files[0].original+'" width=24 height=24 />');
    }catch(e){
        alert('err:'+e.message);
    }
}

//上传图片回调

function callback_upload_waplogo(ret){
    try{
        //alert(ret);
        var json=$.evalJSON(ret);
        if(json.files.length<=0) {
            alert('上传失败');
            return false;
        }
        $("#wap_logo").val(json.files[0].original);
        $("#showcimg").html('<img src="'+json.files[0].original+'" width=24 height=24 />');
    }catch(e){
        alert('err:'+e.message);
    }
}
