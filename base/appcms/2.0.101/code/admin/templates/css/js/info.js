
/**********资讯管理*************/
$(function(){
    C.tabs({"params":
        [
            {"nav":"#bn1","con":"#tab1","sclass":"current","nclass":""},
            {"nav":"#bn2","con":"#tab2","sclass":"current","nclass":""}
        ]
    });
});

//添加编辑办透明层
function show_addinfo(title){
    C.alert.opacty({"width":"880","height":"555","div_tag":"#addinfo","title":title});
}
//资讯：添加或者编辑层
function edit(info_id,act){
    $("#reso_list").html('');//初始化资源图片列表
    switch (act){
    case 'edit':
        $.post("info.php?m=edit",{"info_id":info_id},function(data){
			
            try{
                var json=$.evalJSON(data);
				
                C.form.init([
                    ["#last_cate_id",json.last_cate_id],
                    ["#info_title",json.info_mtitle],
                    ["#info_img",json.info_img],
                    ["#info_stitle",json.info_stitle],
                    ["#info_tags",json.info_tags],
                    ["#info_from",json.info_from],
                    ["#uname",json.uname],
                    ["#info_url",json.info_url],
                    ["#info_desc",json.info_desc],
                    ["#info_id",json.info_id]
                ]);//初始化待编辑数据
				$("#last_cate_id").attr("disabled",true);
                editor1.document.getBody().setHtml(json.info_body);//编辑器赋值
                //遍历输出已有资源图片列表
                if(json.resource.length>0){
                    $("#reso_list").html(show_resource(json.resource));
                }
            }catch(e){
                alert(e.message+'\r\n'+data);
            }
        });

        show_addinfo('编辑资讯');
        break;
    case 'add':
        C.form.init(['.ipt']);//初始化表单
        setTimeout(function(){
            editor1.document.getBody().setHtml('');//初始化编辑框
        },3000);
        show_addinfo('添加资讯');
        break;
    }
}
//提取标签
function get_tags(){
	var info_title = $.trim($("#info_title").val());
	if(!info_title) { C.alert.tips({'content':'请填写标题！'});  return;}
	$.get("info.php?m=tags",{info_title:info_title},function(data){
		try{
			if (!data) return;
			if ($("#info_tags").val().length > 0) {
				if(confirm("确认要替换标签吗？")){
					$("#info_tags").val(data);
				}
			} else {
				$("#info_tags").val(data);
			}
			
		} catch(e) {alert(e.message+'\r\n'+data);}
	});
}
//资讯：保存到数据库
function save(){
    //获取上传图片框
    var resoc=[];
    $(".iptss").each(function(i){resoc.push($(".iptss")[i].value);});

    //获取表单数据
    var postdata=C.form.get_form('#info_form');
    postdata['reso']=resoc.join("#");
    //postdata['info_body']=editor1.document.getBody().getHtml();//编辑框数据
    postdata['info_body']=editor1.getData();//编辑框数据

    //保存数据
    $.post('info.php?m=save',postdata,function(data){
        try{
            var json=$.evalJSON(data);
            C.alert.tips({'content':json.msg});
            var url=window.location.href;
            if(url.indexOf('jsfun=add')>0){
                url=url.replace('jsfun=add','');
            }
            if(json.code==0){
                setTimeout(function(){window.location.href=url},1500);
            }
        }catch(e){alert(e.message+'\r\n'+data);}
    });
}

//资讯：删除选中
function del_check(){
    if(confirm("确认删除吗？")){
        C.form.batch_modify('info.php?m=del','.infoff');
    }
}

//资讯：删除单个
function del(id){
    if(!confirm("确认删除吗？")) return ;
    $.post('info.php?m=del',{"params":id},function(data){
        try{
            var json=$.evalJSON(data);
            C.alert.tips({"content":""+json.msg+""});
            var url=window.location.href;
            if(url.indexOf('jsfun=add')>0){
                url=url.replace('jsfun=add','');
            }
            setTimeout(function(){window.location.href=url},1200);
        }catch(e){alert(e.message+'\r\n'+data);}
    });
}

//资源图片：删除
function delse(id,file){
    if(!confirm("确认删除吗？")) return;
    $.post('info.php?m=delse',{"id":id,"path":file},function(data){
        try{
            var json=$.evalJSON(data);
            C.alert.tips({'content':json.msg});
            if(json.code==0) $("#re_"+id).remove();
        }catch(e){alert(e.message+'\r\n'+data);}
    });
}

//资源图片：上传回调
function callback_upload_thumb(ret){
    try{
        var json=$.evalJSON(ret);
        if(json.files.length<=0) {
            alert('上传失败');
            return false;
        }
        $("#info_img").val(json.files[0].original);
		$("#showimg").html('<img src="'+json.files[0].original+'" width="30" height="30" >');
    }catch(e){alert(e.message+'\r\n'+data);}
}
