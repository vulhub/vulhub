//一级导航 自适应宽
window.onload = function(){
   var obj = getObj("nav-stair").getElementsByTagName('a');
   var aSize = obj.length;
   var aWidth = Math.floor(100/aSize);
   for(var i=0; i< aSize; i++){
       obj[i].style.width = aWidth + '%';
   }
}

//li点击进入详情
function locaUrl(obj){
	window.location.href = obj.getAttribute('url');
}

//js绑定事件
function bindEvent(obj, name, fn)
{
	if(obj.addEventListener){
		obj.addEventListener(name, fn);
	}else if(obj.attachEvent){
		obj.attachEvent(name, fn);
	}
}

//传入ID，设置切换
function tab_info(obj){
	var val = obj.substring(obj.indexOf('-') + 1);
	getObj(obj).className = 't-sel';
	var indexVla = val == 1 ? 2 : (val == 2 ? 1 : 1);
	getObj('item-'+indexVla).className = '';
	getObj('tab-'+ val +'').style.display ='block';
	getObj('tab-'+indexVla).style.display ='none';
}
//获取对象
function getObj(id){
	return document.getElementById(id);
}
