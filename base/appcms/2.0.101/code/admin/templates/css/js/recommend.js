/* 推荐位管理js文件*/

/* 显示添加广告推荐位*/
function recommend_show(area_id,area_type){
    $("#order_tr").css("display","");
    $("#list_tr").css("display","");
    var title;
    switch(area_type){
        case 0:
            title="应用推荐位";
            break;
        case 1:
            title="资讯推荐位";
            break;
        case 2:
            title="广告";
            break;
        case 3:
            title="应用专题";
            break;
        case 4:
            title="资讯专题";
            break;
    }
    if(area_id>0){
        $.post("recommend.php?ajax=json&m=list", {
        "area_id": area_id
    }, function(result) {
        try {
            var ret = $.evalJSON(result);
            var type=ret.recommends.list[0].area_type;
            editor1.document.getBody().setHtml(ret.recommends.list[0].area_html);
            $("#area_id").val(ret.recommends.list[0].area_id);
            $("#area_order").val(ret.recommends.list[0].area_order);
            $("#area_type").val(ret.recommends.list[0].area_type);
            $("#area_logo").val(ret.recommends.list[0].area_logo);
            $("#showimg").html("<img src='"+ret.recommends.list[0].area_logo+"' width=24 height=24/>");
            $("#title").val(ret.recommends.list[0].title);
            $("#data_num").val(ret.recommends.list[0].data_num);
            //$("#area_html").val(ret.recommends.list[0].area_html);
            $("#id_list").val(ret.recommends.list[0].id_list);
            $("#area_remarks").val(ret.recommends.list[0].area_remarks);
            if(type==2){
                $("#order_tr").css("display","none");
                $("#list_tr").css("display","none");
                var width=875;
                var height=400;
            }else if(area_type==1||area_type==0){
                $("#order_tr").css("display","none");
                var width=875;
                var height=500;
            }else{
                var width=875;
                var height=500;
            }
            C.alert.opacty({
                "width": ""+width+"",
                "height": ""+height+"",
                "title": "编辑"+title,
                "div_tag": "#addrecommend_html"
            });
        } catch (e) {
            alert(e.mseeage+'\r\n'+result);
        }
    });
    }else{
        /* area_type==2 是广告位的推荐位*/
        if(area_type==2){
            $("#order_tr").css("display","none");
            $("#list_tr").css("display","none");
            var width=875;
            var height=400;
            editor1.config.startupMode='source';
        }else if(area_type==1||area_type==0){
            $("#order_tr").css("display","none");
            var width=875;
            var height=500;
        }else{
            var width=875;
            var height=500;
        }
        C.form.init(
                    [ ".ipt",
                    ["#area_id","0"],
                    ["#area_order","0"],
                    ["#data_num","1"]
                ]);
        C.alert.opacty({
        "width": ""+width+"",
        "height": ""+height+"",
        "title": "添加"+title,
        "div_tag": "#addrecommend_html"
        });
        $("#area_type").val(area_type);
        return false;
    }
}
/*编辑推荐位*/
function recommend_edit(){
    var href=window.location.href;
    href = href.replace("jsfun=add","");
    //var area_html = editor1.document.getBody().getHtml();
    var area_html = editor1.getData();
    var formvalues=C.form.get_form("areaform");
    formvalues["area_html"]=encodeURIComponent(area_html);
    $.post("recommend.php?m=edit", formvalues,function(result){
        try {
            var ret = $.evalJSON(result), msg = ret.msg;
            ret.code == 0 ? (C.alert.tips({"content":""+msg+""}), setTimeout(function() {
                window.location.href=href;
            }, 1500)) : C.alert.tips({"content":""+msg+""});
        } catch (e) {
            alert(result +e.message);
        }
    });
}

/* 删除友情链接 */
function recommend_del(area_id) {
    var href=window.location.href;
    href = href.replace("jsfun=add","");
    if (!confirm("确认删除")) return;
    $.post("recommend.php?ajax=json&m=del", {
        "area_id": area_id
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


function gethtml(area_id){
    var cont="这个是要生成的代码";
    var html="<textarea cols='26' rows='3'>"+cont+"</textarea>";
    C.alert.opacty({"content":""+html+"","timeout":"20000","width":"300","height":"160"});
}

/* 图片上传回调函数*/
function callback_upload_thumb(ret){
    try{
		//alert(ret);
        var json=$.evalJSON(ret);
        if(json.files.length<=0) {
            alert('上传失败');
            return false;
        }
        $("#area_logo").val(json.files[0].original);
		$("#showimg").html('<img src="'+json.files[0].original+'" width="30" height="30" >');
    }catch(e){
        alert('err:'+e.message);
    }
}

