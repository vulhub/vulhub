$(function(){
    C.tabs({"params":[
        {"nav":"#bn1","con":"#tab1","sclass":"current","nclass":""},
        {"nav":"#bn2","con":"#tab2","sclass":"current","nclass":""},
        {"nav":"#bn3","con":"#tab3","sclass":"current","nclass":""}
        ]
    });
});

//上传图片回调
function callback_upload(ret){
    try{
        //alert(ret);
        var json=$.evalJSON(ret);
        if(json.files.length<=0) {
            alert('上传失败');
            return false;
        }
        $("#cimg").val(json.files[0].original);
        $("#cimg").html('<img src="'+json.files[0].original+'" width=24 height=24 />');
    }catch(e){
        alert('err:'+e.message);
    }
}

//显示添加或者编辑分类层；@cate_id 默认为0，添加；@parent_id 默认为0，顶级栏目
function cate_show(cate_id,parent_id,catetype){
    if(!arguments[0]) cate_id=0;
    if(!arguments[1]) parent_id=0;
    if(!arguments[2]) catetype=0;
    C.form.init(['.ipt',['#cate_id','0'],['#corder','0'],['#parent_id',parent_id],['#catetype',catetype]]);
    if(cate_id==0){
        if(parent_id==0){
            $('#parent_id').attr({'disabled':false});
            $('#catetype').attr({'disabled':false});
        }else{
            $('#parent_id').attr({'disabled':true});
            $('#catetype').attr({'disabled':true});
        }
        C.alert.opacty({"title":"添加分类","width":"500","height":"400","div_tag":"#addcate"});
    }else{
        $('#parent_id').attr({'disabled':true});
        $('#catetype').attr({'disabled':true});
        //读取分类信息并填充到表单
        $.post("category.php?m=get_cate",{cate_id:cate_id},function(data){
            try{
                var json=$.evalJSON(data);
                C.form.init([
                    ['#cate_id',json.cate_id],
                    ['#parent_id',json.parent_id],
                    ['#cname',json.cname],
                    ['#cname_py',json.cname_py],
                    ['#catetype',json.type],
					['#cimg',json.cimg],
					//获取分类显示单选按钮
					[$("input[name=cat_show][value="+json.cat_show+"]").attr("checked", "checked")],
                    ['#corder',json.corder],
                    ['#ctitle',json.ctitle],['#ckey',json.ckey],['#cdesc',json.cdesc],
                    ['#tpl_index',json.tpl_index],['#tpl_listvar',json.tpl_listvar],['#tpl_content',json.tpl_content]
                ]);
            }catch(e){alert('语法错误：'+e.message+'\r\n'+data);}
        });
        C.alert.opacty({"title":"修改分类","width":"500","height":"400","div_tag":"#addcate"});
    }
}
//保存分类数据
function cate_save(){
    var postdata=C.form.get_form('#cate_form');
    $.post("category.php?m=cate_save",postdata,function(data){
        try{
            var json=$.evalJSON(data);
            C.alert.tips({'content':json.msg});
            if(json.code==0){
                setTimeout(function(){
                    window.location.href='category.php';
                },1500);
            }
        }catch(e){alert('语法错误：'+e.message+'\r\n'+data);}
    });
}
//显示移动分类层
function cate_move_show(cate_id,cate_name){
    $('#mycateid').val(cate_id);
    $('#catename').html(cate_name);
    C.alert.opacty({"title":"移动分类","width":"400","height":"160","div_tag":"#movecate"});
}
//移动分类
function cate_move(){
    var mycateid=$('#mycateid').val();
    var moveid=$('#moveid').val();
    $.post("category.php?m=cate_move",{mycateid:mycateid,moveid:moveid},function(data){
        try{
            var json=$.evalJSON(data);
            C.alert.tips({'content':json.msg});
            if(json.code==0){
                setTimeout(function(){
                    window.location.href='category.php';
                },1500);
            }
        }catch(e){alert('语法错误：'+e.message+'\r\n'+data);}
    });
}
//删除分类
function cate_del(cate_id){
    if(!confirm('确定要删除吗？')) return;
    $.post("category.php?m=cate_del",{cate_id:cate_id},function(data){
        try{
            var json=$.evalJSON(data);
            C.alert.tips({'content':json.msg});
            if(json.code==0){
                setTimeout(function(){
                    window.location.href='category.php';
                },1500);
            }
        }catch(e){alert('语法错误：'+e.message+'\r\n'+data);}
    });
}
//修改排序
function cate_order(){
    C.form.update_field('category.php?m=cate_order','.corder','pid');
}
//获取汉字拼音
function get_pinyin(){
    if($('#cname_py').val()==''){
        $.get("category.php?m=get_pinyin&cname="+encodeURIComponent($("#cname").val()),function(data){
            $('#cname_py').val(data);
        });
    }
}

//修正数据
function cate_cdata(){
    C.alert.opacty({"title":"修复文档数量","content":"<br>修复可能耗时较长，请耐心等待"});
    $.post("category.php?m=cate_cdata",{},function(data){
        try{
            C.alert.opacty_close();
            var json=$.evalJSON(data);
            C.alert.tips({'content':json.msg});
            if(json.code==0){
                setTimeout(function(){
                    window.location.href='category.php';
                },1500);
            }
        }catch(e){alert('语法错误：'+e.message+'\r\n'+data);}
    });
	
}