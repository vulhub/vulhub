/* 内链词管理js文件*/
function nlink_show(nlink_id){
    if(nlink_id>0){
        $.post("nlink.php?ajax=json&m=list", {
        "nlink_id": nlink_id
    }, function(result) {
        try {
            var ret = $.evalJSON(result);
            $("#nlink_id").val(ret.nlinks.list[0].nlink_id),
            $("#nlink_txt").val(ret.nlinks.list[0].nlink_txt),
            $("#nlink_url").val(ret.nlinks.list[0].nlink_url),
            C.alert.opacty({
                "width": "305",
                "height": "164",
                "title": "编辑友情链接",
                "div_tag": "#addnlink"
            });
        } catch (e) {
            alert(result + ":e");
        }
    });
    }else{
        C.form.init([ ".ipt" ]);
        C.alert.opacty({
        "width": "305",
        "height": "164",
        "title": "添加内链词",
        "div_tag": "#addnlink"
        });
        return false;
    }
}

/**/
function nlink_edit(){
    var nlink_id=$("#nlink_id").val();
    var nlink_txt=$("#nlink_txt").val();
    var nlink_url=$("#nlink_url").val();
    $.post("nlink.php?m=edit",{
        "nlink_id":nlink_id,
        "nlink_txt":nlink_txt,
        "nlink_url":nlink_url
    },function(result){
        try {
            var ret = $.evalJSON(result), msg = ret.msg;
            ret.code == 0 ? (C.alert.tips({"content":""+msg+""}), setTimeout(function() {
                window.location.reload();
            }, 1500)) : C.alert.tips({"content":""+msg+""});
        } catch (e) {
            alert(result + e.message);
        }
    });
}

/* 删除友情链接 */
function nlink_del(nlink_id) {
    if (!confirm("确认删除")) return;
    $.post("nlink.php?ajax=json&m=del", {
        "nlink_id": nlink_id
    }, function(result) {
        try {
            var ret = $.evalJSON(result), msg = ret.msg;
            if(ret.code == 0){
                C.alert.tips({"content":""+ret.msg+"","timeout":"2000"});
                setTimeout(function(){
                    window.location.reload();
                },1000);
            }else{
                C.alert.tips({"content": "" + ret.msg + ""});
            }
        } catch (e) {
            alert(result + e.message);
        }
    });
}
/* 提交查找 */
function search_submit(url){
    var search_type=$("#search_type").val();
    var search_txt=$("#search_txt").val();
    if(search_txt==''){
        C.alert.tips({"content":"查找内容不能为空"});
        $('#search_txt').focus();
        return false;
    }
    var url = url+'&search_type='+search_type+'&search_txt='+encodeURI(search_txt);
    window.location.href=url;
}