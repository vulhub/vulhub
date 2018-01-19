$(function(){
    product("product");
})

function  product(act){
    var auths=$("#auths").val();
    $.getJSON(data_center_url+'/info.php?jsoncallback=?',{"auth":auths,"act":act},function(json){
        try{   
          if(json.code==0){                  
            if(json.data.length>0){
                html='<table class="tb mt10" style="clear:both;" id="tb-pro">';
                html+='<tr><th width="80">产品ID</th><th>产品名称</th><th>产品介绍</th></tr>';
                $.each(json.data,function(i,v){
                    html+='<tr><td>'+json.data[i].pid+'</td><td><a target="_blank" href="'+json.data[i].purl+'">'+json.data[i].pname+'</a></td><td>'+json.data[i].pdesc+'</td></tr>';
                });
                html+='</table>'
                $("#"+act).html(html);
				var r_height = $('#tb-pro').height() + $('.mfoot').height() + $('.snav').height() + 50;
				var r_body = r_height < $(window).height() - $('.header-wrapper').height() ? $(window).height() - $('.header-wrapper').height() : r_height;

				$('.right_body').height(r_body);
				$('.left_nav').height(r_body);
            }
          }
        }catch(e){alert(e.message);}
    });
}
window.onload = function(){
	layout();
}