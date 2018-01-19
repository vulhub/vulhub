/*关键字中心*/
/*显示修改添加关键字弹出层 */
function keyword_show(keyword_id) {
	
	if (keyword_id <= 0) return C.form.init([ ".ipt", ["#q_order", "0"] ]), $(".div_alert").html(""),
		C.alert.opacty({
		"width": "400",
		"height": "190",
		"title": "添加热门关键字",
		"div_tag": "#addkeyword"
	}), !1;
	
	$.post("keyword.php?ajax=json&m=list",{
		"keyword_id": keyword_id
	}, function(data){
		try{
			var ret = $.evalJSON(data);
			$("#keyword_id").val(ret.keyword.list[0].id);
			$("#q_order").val(ret.keyword.list[0].qorder);
			$("#q_name").val(ret.keyword.list[0].q);
			$("#q_num").val(ret.keyword.list[0].qnum);
			C.alert.opacty({
				"width": "400",
				"height": "190",
				"title": "编辑热门关键字",
				"div_tag": "#addkeyword"
			});
		} catch (e) {
			alert(e.message+data);
		}
	});
}
/* 编辑热门关键字 */
function keyword_edit() {
	var keyword_id = $("#keyword_id").val();
	var q_order = $.trim($("#q_order").val());
	var q_name = $.trim($("#q_name").val());
	var q_num = $.trim($("#q_num").val());
	$.post("keyword.php?ajax=json&m=edit",{"keyword_id":keyword_id,"q_order":q_order,"q_name":q_name,"q_num":q_num},function(data){
		try {
			var ret = $.evalJSON(data), msg = ret.msg;
			ret.code == 0 ? (C.alert.tips({"content":""+msg+""}), setTimeout(function(){
				window.location.reload();
			},1000)) : C.alert.tips({"content":""+msg+""});
		} catch (e) {
			alert(e.message+data);
		}
	});
}
/* 删除友情链接 */
function keyword_del(keyword_id) {
    if (!confirm("确认删除")) return;
    $.post("keyword.php?ajax=json&m=del", {
        "keyword_id": keyword_id
    }, function(data) {
        try {
            var ret = $.evalJSON(data), msg = ret.msg;
            if(ret.code == 0){
                C.alert.tips({"content":""+ret.msg+""});
                setTimeout(function(){
                    window.location.href = "keyword.php"
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
    var search_txt=$.trim($("#search_txt").val());
    if(search_txt==''){
        C.alert.tips({"content":"查找内容不能为空"});
        $('#search_txt').focus();
        return false;
    }
    var url = url+'&q='+encodeURI(search_txt);
    window.location.href=url;
}