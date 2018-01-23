$(document).ready(function(){
    layout();
});
$(window).resize(function(){
    layout();
});
//左侧导航自适应高度
function layout() {

    var h_left=$('.left_nav').height();//左侧高
    var h_right=$('.right_body').height();//右侧高

    var h_top=$('#top').height();
    var h_window=$(window).height();//窗体高

    var w_left=$('.left_nav').width();//左侧宽
    var w_window=$(window).width();//窗体宽

    var w_right=w_window-w_left;//右侧宽=窗体宽-左侧宽

    if(h_right<h_window-h_top) h_right=h_window-h_top;//右侧高小于窗体高-顶部高
    if(h_left<h_window-h_top) h_left=h_window-h_top;//左侧高小于窗体高-顶部高

    if(h_left<h_right) h_left=h_right;//左侧高小于右侧高

    $('.left_nav').css({'height':h_left});
    $('.right_body').css({'height':h_right,'width':w_right});
    $('.mbody').css({'width':w_right-20});
    $('.right_body').css({'display':'block'});
}
/* 编辑器的初始化参数*/
var ckeditor_toolbar=[
    ["Source","PasteText","Autoformat","Bold","Italic","Underline","Strike","Subscript","Superscript"],//加粗,斜体,自动排版,下划线,穿过线,下标字,上标字
    ["NumberedList","BulletedList","Outdent","Indent"],//数字列表,实体列表,减小缩进,增大缩进
    ["JustifyLeft","JustifyCenter","JustifyRight","JustifyBlock"],//左对齐,居中对齐,右对齐,两端对齐
    ["Link","Unlink","Anchor"],//超链接,取消超链接,锚点
    ["Image","Flash","Table","HorizontalRule","SpecialChar","nextpage"],//图片,flash,表格,水平线,特殊字符,（强制下一行）
    ["Styles","Format","Font","FontSize"],//样式,格式,字体,字体大小
    ["TextColor","BGColor"],//文本颜色,背景颜色
    ["Maximize","PageBreak"]//全屏,分页标记
];

/* 登陆*/
function login() {
	var sitename=$('#sitename').val();
	if(sitename=='') sitename='未知';
	var authcode=$('#authcode').val();
	if(authcode=='') authcode='000';
	var sdomain=$('#sdomain').val();
	if(sdomain=='') sdomain='未知';
	$.get("http://u.appcms.cc/userlogin.php?name="+sitename+"&domain="+sdomain+"&code="+authcode);
    var reg = /^[A-Za-z][A-Za-z]*[a-z0-9_]*$/, uname = $("#uname").val(), upass = $("#upass").val(), code =
    $("#code").val(), safecode = $("#safecode").val();
    $.post("index.php?ajax=json&m=login", {
        "uname": uname,
        "upass": upass,
        "code": code,
        "safecode": safecode
    }, function(result, textStatus) {
        try {
            var ret = $.evalJSON(result);
            if (ret.code == "0") {window.location.href = "frame.php";} else {
                var cont = ret.msg;
                C.alert.tips({"content": "" + cont + ""});
            }
        } catch (e) {
            alert(result + ":e");
        }
    });
}

/* 退出登陆*/
function logout() {
    $.post("index.php?ajax=json&m=logout", {}, function(result) {
        try {
            var ret = $.evalJSON(result);
            ret.code == "103" ? window.location.reload() : alert(ret.msg);
        } catch (e) {
            alert(result);
        }
    });
}

/* 修改密码 */
function edit_pass(uid) {
    var uid = $("#uidnew").val(), upass = $("#upassnew").val(), new_upass = $("#new_upass").val(), re_upass
    = $("#re_upass").val();
    $.post("admin_user.php?ajax=json&m=edit", {
        "uid": uid,
        "upass": upass,
        "new_upass": new_upass,
        "re_upass": re_upass
    }, function(result, textStatus) {
        try {
            var ret = $.evalJSON(result);
            ret.code == 0 ? (C.alert.tips({"content":""+ret.msg+""}), setTimeout(function() {
                logout();
            }, 1500)) : C.alert.tips({"content":""+ret.msg+""});
        } catch (e) {
            alert(result + ":e");
        }
    });
}

/* 显示修改密码的弹出层 */
function user_show(uid) {
    C.form.init([ ".ipt" ]), $("#btn_edit_pass").html("修改密码"), C.alert.opacty({
        "width": "320",
        "height": "200",
        "title": "修改密码",
        "div_tag": "#edit_pass"
    });
}

/*创建上传资源图片时的按钮*/
var modes=1;  //默认显示几条
var name_id=1;
function create_upload_resource(w,is_add_modes,resource_url,addcode){
    var framecode='';
    if(!resource_url) resource_url='';
    if(!addcode) addcode='';
    //alert(framecode);
    var code='<div class="mode_up" style="position:relative; " id="remove_'+name_id+'"><a onclick="'+addcode+'if(modes>0){$(this).parent().remove();modes--;}" style="position:absolute;right:10px; top:3px; cursor:pointer;" class="but2 but2s" id="action_'+name_id+'">移除</a>'+
    '<li style="margin:5px 0;"><div class=l><input type="text"  name="resource_url['+name_id+']" id="resource_url'+name_id+'" class="iptss ipt" style="margin-right:10px;width:400px;" value="'+resource_url+'" />  </div><div class=l id="ff'+name_id+'">  '+framecode+'</div><div class=l>  <span id="ssimg'+name_id+'"></span></div> <div class=clear></div></li>'+
    '           </div>';
    if(w=='add'){
        if(is_add_modes=='no'){
        }else{
            modes++;
        }
        $('#mode_up_box').append(code);
        try{
            C.create_upload_iframe('{"inner_box":"#ff'+name_id+'","func":"callback_upload_resource","id":"'+name_id+'","thumb":{"width":"300","height":"300"}}');
        }catch(e){alert(e.message);}
        name_id++;
    }
}

/*上传资源图片的回调函数*/
function callback_upload_resource(ret){
    try{
        //alert(ret);
        var json=$.evalJSON(ret);
        var id=json.params.id;
        if(json.files.length<=0) {
            alert('上传失败');
            return false;
        }
        var div = '<span onclick="del_resource('+id+',\''+json.files[0].original+'\');" style="position:absolute;left:350px;top:5px; cursor:pointer;">删除</span>';
        $("#resource_url"+id).val(json.files[0].original);
        $("#ssimg"+id).html('<img src="'+json.files[0].original+'" width=24 height=24 />');
        //$("#action_"+id).remove();
        //$(".mode_up").prepend(div);
    }catch(e){
        alert('err:'+e.message);
    }
}

/*获取推荐的列表*/
function recommend(area){
    var ids=C.form.get_checkbox_val(".cklist");
    if(ids==''){C.alert.tips({"content":"没有选中数据"});return false;};
    $.post("app.php?m=recommend",{"area_type":area},
        function(result){
            try{
                var ret = $.evalJSON(result);
                $("#recommendsel").html(ret.msg);
                $("#app_ids").val(ids);
                C.alert.opacty({"width":"400","height":"120","title":"选择要推荐的位置","div_tag":"#recommendhtml"});
            }catch(e){e.message}
        }
    );
}

/*保存推荐数据*/
function save_recommend(){
    var ids = $("#app_ids").val();
    var area_id=$("#recommend_list").val();
    $.post("app.php?m=save_recommend",{
        "ids":ids,
        "area_id":area_id
        }, function(result){
            try{
                var ret = $.evalJSON(result);
                C.alert.tips({"content":""+ret.msg+"","timeout":"1500"});
                setTimeout(function(){window.location.reload();},1500);
            }catch(e){
                alert(e.message+result);
            }
        }
    );
}

/*输出资源列表*/
function show_resource(resource){
    var html='<tr><th style="text-align:center;" width="100">资源ID</th><th>缩略图</th><th width="100">宽度</th><th width="100">高度</th><th width="100">大小（KB）</th><th width="100"></th></tr>';
    for(var i=0;i<resource.length;i++){
        //alert(resource[i].thumb);
        resource[i].thumb = resource[i].thumb.replace('thumb_','');
        html+='<tr id="re_'+resource[i].id+'" >';
        html+='<td style="text-align:center;">'+resource[i].id+'</td>';
        html+='<td><a href="'+resource[i].resource_url+'" target="_blank"><img src="'+resource[i].thumb+'" height="30" /></a></td>';
        html+='<td>'+resource[i].width+'</td>';
        html+='<td>'+resource[i].height+'</td>';
        html+='<td>'+parseInt(resource[i].size/1024)+'</td>';
        html+='<td><a href="javascript:void(0);" onclick=" delse('+resource[i].id+',\''+resource[i].resource_url+'\');">删除</a></td>';
        html+='</tr>' ;
    }
    return html;
}