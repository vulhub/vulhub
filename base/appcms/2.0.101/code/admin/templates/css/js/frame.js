$(function(){
    ad_saf_team("adlist");
    ad_saf_team("safe_tips");
    ad_saf_team("dev_team");
})

function  ad_saf_team(act){
    var auths=$("#auths").val();
    $.getJSON(data_center_url+'/info.php?jsoncallback=?',{"auth":auths,"act":act,"ver":appcms_ver},function(json){
        try{    
            if(json.code==0){ 
                if(json.data!=''){
                    var htmls='';
                    $.each(json.data,function(i,v){
                        if(json.data[i].url==''){
                            htmls+='<li>'+json.data[i].html+'</li>';
                        }else{
                            htmls+='<li><a href="'+json.data[i].url+'" target="_blank">'+json.data[i].html+'</a></li>';
                        }
                    });

                    $("#"+act).html(htmls);
                    //重算高度
                    var tHeight=$("#adlist").height()+$("#safe_tips").height()+$("#dev_team").height()+280;
                    var rHeight = tHeight > $(window).height() ?  tHeight : $(window).height();
                    $('.right_body').css({'height': rHeight });
                }
            }
        }catch(e){alert(e.message);}
    });
}