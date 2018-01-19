/*显示添加模板名称*/
function show_temp(){
    C.alert.opacty({"width":"380","height":"120","div_tag":"#addtemplate","title":"添加模板文件"});
}
/*保存模板*/
function save_temp(){
    var file = $("#tempname").val();
    $.post("template.php?m=create_file",{
        "filename":file
    },function(result){
        try {
            var ret = $.evalJSON(result);
            if(ret.code == 0 ){
                C.alert.tips({"content":""+ret.msg+""});
                setTimeout(function(){
                    window.location.href="template.php?m=show&filename="+ret.filename;},1500);
            }else{
                C.alert.tips({"content":""+ret.msg+""});
            }
        } catch (e) {
            alert(e.message+"::"+result);
        }
    });
}