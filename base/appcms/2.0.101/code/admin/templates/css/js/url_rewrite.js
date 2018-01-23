/* 内链词管理js文件*/
function url_show(url_id){
    if(url_id>0){
        $.post("url_rewrite.php?ajax=json&m=list", {
        "url_id": url_id
    }, function(result) {
        try {
            var ret = $.evalJSON(result);
            $("#url_id").val(ret.url_rewrites.list[0].url_id);
            $("#url_title").val(ret.url_rewrites.list[0].url_title);
            $("#url_true").val(ret.url_rewrites.list[0].url_true);
            $("#url_rule").val(ret.url_rewrites.list[0].url_rule);
            $("#url_title").attr("disabled",true);
            $("#url_true").attr("disabled",true);
            C.alert.opacty({
                "width": "400",
                "height": "272",
                "title": "编辑规则节点",
                "div_tag": "#addurl"
            });
        } catch (e) {
            alert(result + ":e");
        }
    });
    }else{
        $("#url_title").attr("disabled",false);
        $("#url_true").attr("disabled",false);
        C.form.init([ ".ipt" ]);
        C.alert.opacty({
        "width": "400",
        "height": "272",
        "title": "添加规则节点",
        "div_tag": "#addurl"
        });
        return false;
    }
}

/* 编辑添加伪静态规则*/
function url_edit(){
    var formvalues = C.form.get_form("addurlform");
    $.post("url_rewrite.php?m=edit",formvalues,function(result){
        try {
            var ret = $.evalJSON(result), msg = ret.msg;
            ret.code == 0 ? (C.alert.tips({"content":""+msg+""}), setTimeout(function() {
                window.location.reload();
            }, 1500)) : C.alert.tips({"content":""+msg+""});
        } catch (e) {
            alert(result + ":e");
        }
    });
}

/* 删除友情链接 */
function url_del(url_id) {
    if (!confirm("确认删除")) return;
    $.post("url_rewrite.php?ajax=json&m=del", {
        "url_id": url_id
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