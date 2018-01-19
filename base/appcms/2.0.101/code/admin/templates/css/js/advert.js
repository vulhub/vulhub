
// JavaScript Document
//添加广告位 div
function show_ad_pace(){
    $("#title").val('');
    $("#area_remarks").val('');
    $("#type").val('1');
     $("#type").attr("disabled",false); 
    C.alert.opacty({"width":"400","height":"270","div_tag":"#advert_html","title":"添加广告位"});
}
//添加广告 div
function show_ad(id,type){
    //广告位id
    $(".ad_type").val(type);
    $(".ad_pace").val(id);
    $.post('advert.php?m=edit_ads',{"area_id":id},function(data){
        try{
            var json=$.evalJSON(data);
            var ads=json.area_html;
            if(type==1){//文字广告
                ads=ads.replace(/&#34;/g,'"');
                $("#area_html").val(ads);
                C.alert.opacty({"width":"530","height":"300","div_tag":"#font_adverts","title":"管理广告"});
            }else{
                $("#contents").html('');
                trlen=ads.length
                html='';
                for(var i=0;i<trlen;i++){
                    html+='<tr id="tr_'+i+'" >';
                    html+='<td><input id="title" class="ipt" value="'+ads[i].title+'" style="width:80px" /></td>';
                    html+='<td><input id="desc" class="ipt" value="'+ads[i].desc+'" /></td>';
                    html+='<td><input id="url" class="ipt" value="'+ads[i].url+'" style="width:130px"/></td>';
                    html+='<td><div class=l><input type="text"  name="resource_url['+i+']" id="resource_url" class="ipt" style="margin-right:10px;" value="'+ads[i].resource_url+'"/></div><div class="l" id="ssimg'+i+'"></div><div class=l id="ff'+i+'"><script> C.create_upload_iframe(\'{"inner_box":"#ff'+i+'","func":"callback_upload_resource1","id":"'+i+'","thumb":{"width":"300","height":"300"}}\');</script></div> </td>';
                    html+='<td align="right"><a style="float:right;margin-right:0;" href="javascript:void(0);" class="but2" id="subtn" onclick="del_ad_html('+i+');" >删除</a></td>';
                    html+='</tr>';
                }
                $("#contents").append(html);    
                C.alert.opacty({"width":"850","height":"300","div_tag":"#fouce_advarts","title":"管理广告"});
            }
        }catch(e){alert(e.message+data);}
    })
    
}
//    
    
    
    
//添加广告html    
function add_ad_html(){
    html='';
    var trlen=$("#contents > tr").length
    htmls(trlen);
    $("#contents").append(html);
}
    
function htmls(trlen){
    html+='<tr id="tr_'+trlen+'" >';
    html+='<td><input id="title" class="ipt" style="width:80px" /></td>';
    html+='<td><input id="desc" class="ipt"  /></td>';
    html+='<td><input id="url" class="ipt" style="width:130px" /></td>';
    html+='<td><div class=l><input type="text"  name="resource_url['+trlen+']" id="resource_url" class="ipt" style="margin-right:10px;" /></div><div class="l" id="ssimg'+trlen+'"></div><div class=l id="ff'+trlen+'"><script> C.create_upload_iframe(\'{"inner_box":"#ff'+trlen+'","func":"callback_upload_resource1","id":"'+trlen+'","thumb":{"width":"300","height":"300"}}\');</script></div> </td>';
    html+='<td><a href="javascript:void(0);" style="float:right;margin-right:0;"  class="but2" id="subtn" onclick="del_ad_html('+trlen+');" >删除</a></td>';
    html+='</tr>';
    return html;
}
    
//删除广告html
function del_ad_html(id){
    $("#tr_"+id).remove();
    C.alert.tips({"content":"删除成功"});
    save_ads();
}    
    

    
//保存广告列表
function save_ads(){
    var adpaceid=$(".ad_pace").val();
    var type=$(".ad_type").val();
    if(type==1){
        temp=$("#area_html").val();
    }else if(type==2){
        var trlen=$("#contents > tr").length;
        var temp=[];
        for(var i=0;i<trlen;i++){
            val=C.form.get_form("#tr_"+i);
            temp.push(val);
        }
        temp=$.toJSON(temp);
    }
    $.post('advert.php?m=save_ads',{"list":temp,"type":type,"area_id":adpaceid},function(data){
         try{
            json=eval('('+data+')');
            C.alert.tips({"content":""+json.msg+""});
            if(json.code==0){
                setTimeout(function(){window.location.reload()},1500);
            }
            }catch(e){alert(e.message+data);}        
    });

    
}


//编辑广告位
function edit_adpace(area_id){
	$("#area_id").val(area_id);
    $.post('advert.php?m=edit_adpace',{'area_id':area_id},function(data){
        try{
            var json=$.evalJSON(data);
            $("#title").val(json.title);
            $("#area_remarks").val(json.area_remarks);
            $("#type").val(json.type);
            $("#type").attr("disabled","false");
            C.alert.opacty({"width":"400","height":"270","div_tag":"#advert_html","title":"编辑广告位"});
        }catch(e){alert(e.message+data);}
    })
    
}    
//保存广告位
function save_adpace(){
    var adpace=C.form.get_form("#areaform");
    $.post('advert.php?m=save_adpace',adpace,function(data){
        try{
            json=$.evalJSON(data);
            C.alert.tips({"content":""+json.msg+""});
            if(json.code==0){
                setTimeout(function(){window.location.reload()},1500);
            }
        }catch(e){alert(e.message+data);}
    });
}

//删除广告位
function del_adpace(area_id) {
    if (!confirm("确认删除")) return;
    $.post("advert.php?m=del_adpace", {
        "area_id": area_id
    }, function(data) {
        try {
            var json = $.evalJSON(data);
                C.alert.tips({"content":""+json.msg+""});
                if(json.code == 0){
                setTimeout(function(){
                    window.location.reload();
                },1000);
               }
        } catch (e) {
            alert(result + e.message );
        }
    });
}

//图片上传回调函数
function callback_upload_resource1(ret){
    try{
        //alert(ret);
        var json=$.evalJSON(ret);
        var id=json.params.id;
        if(json.files.length<=0) {
            alert('上传失败');
            return false;
        }
       
        $("#ff"+id).parent().find("input").val(json.files[0].original);
        $("#ssimg"+id).html('<img src="'+json.files[0].original+'" width=24 height=24 />');
    }catch(e){
        alert('err:'+e.message);
    }
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
