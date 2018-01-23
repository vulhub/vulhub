/*友情链接中心*/
//上传图片回调
function callback_upload(ret){
    try{
        //alert(ret);
        var json=$.evalJSON(ret);
        if(json.files.length<=0) {
            alert('上传失败');
            return false;
        }
        $("#flink_img").val(json.files[0].original);
        $("#showcimg").html('<img src="'+json.files[0].original+'" width=24 height=24 />');
    }catch(e){
        alert('err:'+e.message);
    }
}
/*显示修改添加友链弹出层 */
function flink_show(flink_id) {
    if (flink_id <= 0) return C.form.init([ ".ipt", [ "#flink_order", "0" ],["#flink_url","http://"] ]), $(".div_alert").html(""),
    $("input[name=flink_is_site][value=0]").attr("checked", "checked"), C.alert.opacty({
        "width": "395",
        "height": "276",
        "title": "添加友情链接",
        "div_tag": "#addflink"
    }), !1;
    $.post("flink.php?ajax=json&m=list", {
        "flink_id": flink_id
    }, function(result) {
        try {
            var ret = $.evalJSON(result);
            $("#flink_id").val(ret.flinks.list[0].flink_id),
            $("#flink_order").val(ret.flinks.list[0].flink_order),
            $("#flink_name").val(ret.flinks.list[0].flink_name),
            $("#flink_url").val(ret.flinks.list[0].flink_url),
            $("#flink_img").val(ret.flinks.list[0].flink_img),
            $("input[name=flink_is_site][value="+ret.flinks.list[0].flink_is_site+"]").attr("checked", "checked"), C.alert.opacty({
                "width": "395",
                "height": "276",
                "title": "编辑友情链接",
                "div_tag": "#addflink"
            });
        } catch (e) {
            alert(result + ":e");
        }
    });
}
/* 编辑友情链接 */
function flink_edit() {
    var flink_id = $("#flink_id").val(), flink_order = $("#flink_order").val(), flink_name =
    $("#flink_name").val(), flink_url = $("#flink_url").val(), flink_img = $("#flink_img").val(),
    flink_is_site = $("input[name='flink_is_site']:checked").val();
    $.post("flink.php?ajax=json&m=edit", {
        "flink_id": flink_id,
        "flink_order": flink_order,
        "flink_name": flink_name,
        "flink_url": flink_url,
        "flink_img": flink_img,
        "flink_is_site": flink_is_site
    }, function(result) {
        try {
            var ret = $.evalJSON(result), msg = ret.msg;
            ret.code == 0 ? (C.alert.tips({"content":""+msg+""}), setTimeout(function() {
                window.location.reload();
            }, 1000)) : C.alert.tips({"content":""+msg+""});
        } catch (e) {
            alert(result + ":e");
        }
    });
}
/* 删除友情链接 */
function flink_del(flink_id) {
    if (!confirm("确认删除")) return;
    $.post("flink.php?ajax=json&m=del", {
        "flink_id": flink_id
    }, function(result) {
        try {
            var ret = $.evalJSON(result), msg = ret.msg;
            if(ret.code == 0){
                C.alert.tips({"content":""+ret.msg+""});
                setTimeout(function(){
                    window.location.href = "flink.php"
                },1000);
            }else{
                C.alert.tips({"content": "" + ret.msg + ""});
            }
        } catch (e) {
            alert(result + ":e");
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