//提交给自动备份数据
function backall(url){
	var table = "";
	$("input[name=table[]][checked]").each(function(){
		table +=$(this).val()+",";
	});
	tables = table.slice(0,table.lastIndexOf('no')-3);
	if(!tables) { C.alert.tips({"content":"请选择要备份的数据表"}); return}
	var url = url+"&table="+tables+"&first=1";
    $("#autobackiframe").attr({"src":url});
}



//ajax请求备份数据库
/*
function backup(url){
	var table = "";
	$("input[name=table[]][checked]").each(function(){
		table +=$(this).val()+",";
	});
	tables = table.slice(0,table.lastIndexOf('no')-3);
	if(!tables) { C.alert.tips({"content":"请选择要备份的数据表"}); return}
	//C.alert.opacty({"content":"<div style='margin-top:15px;'>正在备份数据库,大数据量需要耗时请耐心等待....</div>"});
	$.post("dbbak.php?m=save",{table:tables},function(data){
		alert(data);
		try{
			
			var obj = eval("("+data+")");
			if(obj.msg > 0) {
				C.alert.opacty({"content":"<div style='margin-top:15px;'>"+obj.msg+"</div>"});
				setTimeout(function(){
					window.location.reload();
				},1200);
			}
		}catch(e){
			alert(e.message+data);
		}
		
	});
	
}*/
//ajax请求导入数据库
function backin(url){
	/*
	var table = "";
	$("input[type=checkbox]").each(function(){
		table +=$(this).val()+",";
	});
	tables = table.slice(0,-1);
	var url = url+"&table="+tables;*/
    $("#autobackiframe").attr({"src":url});
	
}
//ajax请求删除备份数据库文件
function backdel(){
	$.get("dbbak.php?m=del",{},function(data){
		var obj = eval("("+data+")");
		C.alert.tips({"content":"<div style='margin-top:15px;'>"+obj.msg+"</div>"});
		setTimeout(function(){
			window.location.reload();
		},1200);
	});
}
//全选
function CheckAll(form){
	for (var i=0;i<form.elements.length;i++){
	var e = form.elements[i];
	if (e.Name != 'chkAll'&&e.disabled==false)
		e.checked = form.chkAll.checked;
	}
	$("#chkallno").attr("checked",'');
	$("#chkAll").attr("checked",true);
}
//全不选
function CheckAllNO(form){
	for (var i=0;i<form.elements.length;i++){
	var e = form.elements[i];
	if (e.Name != 'chkAll'&&e.disabled==false)
	e.checked = form.chkAll.checked;
	}
	$("#chkAll").attr("checked",'');
	$("#chkallno").attr("checked",true);
}