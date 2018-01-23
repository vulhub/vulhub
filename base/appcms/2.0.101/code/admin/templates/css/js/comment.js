//删除评论
function del(id){
    if(!confirm('确定要删除吗？')) return;
    $.post('comment.php?m=del',{"params":id},function(json){
        try{
            var json=eval('('+json+')');
            C.alert.tips({"content":""+json.msg+""});
            setTimeout(function(){
                window.location.reload();
            },1200);
        }catch(e){
        alert(e.message+json);
        }
    });
}
//更改状态，屏蔽或者通过
function edit(id,ischeck){
    $.post('comment.php?m=edit&is_check='+ischeck,{"params":id},function(json){
        try{
            var json=eval('('+json+')');
            C.alert.tips({"content":""+json.msg+""});
            setTimeout(function(){
                window.location.reload();
            },1200);
        }catch(e){
        alert(e.message+json);
        
        }
    });
}
