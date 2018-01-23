var index = 0;    //关键字列表项索引
var count = 0;    //关键字列表总数
var status =false;
var v = null;
function setText(x,obj){
	$(obj).val(x);
	if($("#searchLayer").length > 0 ){ $("#searchLayer").remove(); }
	
}
function reSearch(obj){
    if($(obj).val() != ""){                       //如果用户录入的关键字不为空
      $.get("index.php?q="+encodeURI($.trim($(obj).val()), "utf-8"),function(data){
                                                          //通过jQuery向服务器发送GET请求
            if($.trim(data) == ""){                       //如果返回数据为空，则隐藏下拉列表层
				 $("#searchLayer").remove();
            }else{
				var json = eval("("+data+")");
				v = obj;
				var html = '';
				for(var i=0;i< json.length;i++) {
					html +='<div id="listItem_' + i + '" style="height:30px; clear:both; background-color:#FFFFFF; cursor:pointer;padding:0 20px;" onmouseover="this.style.backgroundColor=\'#1E9AE0\';this.style.color=\'#fff\';" onmouseout="this.style.backgroundColor=\'#FFFFFF\';this.style.color=\'#000\';" onclick="setText(\'' +json[i].app_title + '\',v); window.location.href=\'index.php?tpl=search&q=\'+$(v).val();">';;
					html +='    <li  style="width:100%; height:30px; line-height:30px; text-align:left; float:left;"><span id="listItem_li_' + i + '" style="text-align:left; float:left;display:inline-block; overflow:hidden; width:'+ parseInt((v.width- 42)/2,10)  +'px;height:30px;">' + json[i].app_title + '</span><span style="text-align:right; float:right;display:inline-block;height:30px;  overflow:hidden; width:'+ parseInt((v.width- 42)/2,10)  +'px;">' + json[i].app_down + '</span></li>';
					html +='</div>'; 
				}
				//删除已存在的DIV
				if($("#searchLayer").length >= 0) {
					$("#searchLayer").remove();
				} 
				//创建空DIV
				CreateDiv(obj);
				$("#searchLayer").html(html);             //如果返回数据不为空则显示下拉列表
				moveOutHide();
            	count = parseInt(json.length);   //获取列表中关键字个数
            }
        });
    }
}
function moveOutHide(){
	if($("#searchLayer").length>0){
		$("#searchLayer").bind('mouseleave',function(){
			$(this).remove();

		});
	}
}
function CPos(x, y){
    this.x = x;
    this.y = y;
}
//动态创建div
function CreateDiv(obj) {
	//获取当前对象父节点
	var p = obj.parentNode;
	p.style.position = "relative";
	p.style.zIndex="9999";
	var target = obj;
    var pos = new CPos(target.offsetLeft, target.offsetTop);
    var target = target.offsetParent;
    while (target) {
        pos.x += target.offsetLeft;
        pos.y += target.offsetTop;
        target = target.offsetParent
    }
	
	var box = document.createElement("div");
	box.style.backgroundColor = "#FBFCFC";
	box.style.position="absolute";
	box.style.display = "block";
	box.style.left = "0";
	box.style.border="1px solid #ccc";
	box.style.borderWidth="0 1px 1px 1px";
    box.style.top= obj.clientHeight+3+"px";
    box.style.width=obj.offsetWidth - 2 +"px" ;
	box.id = "searchLayer";
	if(obj.style.marginLeft != null && obj.style.marginLeft != ""){
		box.style.marginLeft = obj.style.marginLeft;
	}
    p.appendChild(box);
	return box;
}
/*
function search_skip(obj){
	CreateDiv(obj);
	moveOutHide();
}*/
//搜索框为空鼠标点击显示15个热搜词
function show_hot(obj){
	if($("#searchLayer").length <= 0) {
		CreateDiv(obj);
	}
	if($.trim($(obj).val()) == '') {
	    var q = $.trim($(obj).val());
		$.get("index.php?act=hot",{q:q},function(data){
			var json = eval("("+data+")");
			var html = "";
			v = obj;
			
			for (var i=0;i<json.length;i++){
				html +='<div id="listItem_' + i + '" style="height:30px; clear:both; background-color:#FFFFFF; cursor:pointer;padding:0 20px;" onmouseover="this.style.backgroundColor=\'#1E9AE0\';this.style.color=\'#fff\';" onmouseout="this.style.backgroundColor=\'#FFFFFF\';this.style.color=\'#000\';" onclick="setText(\'' + json[i]['q'] + '\',v); window.location.href=\'index.php?tpl=search&q=\'+$(v).val();">';
				html +='    <li  style="width:100%; height:30px; line-height:30px; text-align:left; float:left;"><span id="listItem_li_' + i + '" style="text-align:left; float:left; display:inline-block; overflow:hidden; width:'+ parseInt((v.width- 42)/2,10)  +'px;height:30px;">' + json[i]['q'] +'</span><span style="text-align:right; float:right;display:inline-block; overflow:hidden; height:30px; width:'+ parseInt((v.width- 42)/2,10)  +'px;">'+ json[i]['qnum'] +'</span></li>';
				html += '</div>';
			}
			$("#searchLayer").html(html);
			$("#searchLayer").mouseleave(function(){
				$("#searchLayer").remove();
			});
		});
	}
}

//文本框输入内容
function search_up(obj,e){
		var evt = e || window.event; 
		//13回车键  40向下键  38向上键
		if (evt.keyCode == 40 || evt.keyCode == 38) return false;
		if	(evt.keyCode == 13) {
			$("#searchLayer").remove();
			do_search();
		}
			index = 0; 
			$("#searchLayer").remove();
			v = obj;
			
			if ($(obj).val() == '' || $.trim($(obj).val()) == '') {
				$("#searchLayer").remove();
				return false;
			} else {
				setTimeout("reSearch(v)", 150);//否则每隔150毫秒调用一次reSearch()方法	
			}			
}
//按上下键选择
function search_down(obj,e){
		var e = e || window.event; 
		var keycode = e.keyCode;                   //获取用户录入字符的ASCII
		if(keycode == 40){                        //如果用户按向下方向键
			if (status && index == 0) {
				index++; 
			}
			//更改当前所选项的背景色
			//更改当前所选项的前景色
			$("#listItem_"+index).css({ color: "#fff", background: "#1E9AE0" });
			$(obj).val($.trim($("#listItem_li_"+index).html())); //将当前所选项显示在文本框中
			if(index >0){                   
				$("#listItem_"+parseInt(index-1)).css("background", "#fff");//更改当前项前一项的背景色
				$("#listItem_"+parseInt(index-1)).css("color", "#333333");//更改当前项前一项的前景色
				status = true;
			} 
			if(index < count-1){//如果当前索引小于总关键字数减1
				index++;       //索引数增1
			}
		}else if(keycode == 38){//如果用户按向上方向键	
			if(index < count){
				$("#listItem_"+parseInt(index)).css("background", "#fff");//更改当前项前一项的前景色
				$("#listItem_"+parseInt(index)).css("color", "#333333"); //更改当前项前一项的前景色
			} 
			if(index > 0){//如果当前索引小于总关键字数减1
				index--; //索引数增1
			}
			$(obj).val($.trim($("#listItem_li_"+index).html())); //将当前所选项显示在文本框中
			$("#listItem_"+parseInt(index)).css({ color: "#fff"});
			$("#listItem_"+parseInt(index)).css("background", "#1E9AE0");	
		}

}





