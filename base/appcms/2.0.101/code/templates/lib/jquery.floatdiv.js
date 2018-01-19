/*任意位置浮动固定层*/
/*没剑(http://regedit.cnblogs.com) 08-03-11*/
/*说明：可以让指定的层浮动到网页上的任何位置，当滚动条滚动时它会保持在当前位置不变，不会产生闪动*/

/*2008-4-1修改：当自定义right位置时无效，这里加上一个判断
有值时就不设置，无值时要加18px已修正层位置在ie6下的问题
*/
/*调用：
1 无参数调用：默认浮动在右下角
$("#id").floatdiv();

2 内置固定位置浮动
//右下角
$("#id").floatdiv("rightbottom");
//左下角
$("#id").floatdiv("leftbottom");
//右下角
$("#id").floatdiv("rightbottom");
//左上角
$("#id").floatdiv("lefttop");
//右上角
$("#id").floatdiv("righttop");
//居中
$("#id").floatdiv("middle");

3 自定义位置浮动
$("#id").floatdiv({left:"10px",top:"10px"});
以上参数，设置浮动层在left 10个像素,top 10个像素的位置



*/
jQuery.fn.floatdiv=function(location){
	//ie6要隐藏纵向滚动条
	var isIE6=false;
	if($.browser.msie && $.browser.version=="6.0"){
		$("html").css("overflow-x","auto").css("overflow-y","hidden");
		isIE6=true;
	};
	$("body").css({margin:"0px",padding:"0 0 0 0",
		border:"0px",
		height:"100%",
		overflow:"auto"
	});
	return this.each(function(){
		var loc;//层的绝对定位位置
		if(location==undefined || location.constructor == String){
			switch(location){
				case("rightbottom")://右下角
					loc={right:"0px",bottom:"0px"};
					break;
				case("leftbottom")://左下角
					loc={left:"0px",bottom:"0px"};
					break;	
				case("lefttop")://左上角
					loc={left:"0px",top:"0px"};
					break;
				case("righttop")://右上角
					loc={right:"0px",top:"0px"};
					break;
				case("middle")://居中
					var l=0;//居左
					var t=0;//居上
					var windowWidth,windowHeight;//窗口的高和宽
					//取得窗口的高和宽
					if (self.innerHeight) {
						windowWidth=self.innerWidth;
						windowHeight=self.innerHeight;
					}else if (document.documentElement&&document.documentElement.clientHeight) {
						windowWidth=document.documentElement.clientWidth;
						windowHeight=document.documentElement.clientHeight;
					} else if (document.body) {
						windowWidth=document.body.clientWidth;
						windowHeight=document.body.clientHeight;
					}
					l=windowWidth/2-$(this).width()/2;
					t=windowHeight/2-$(this).height()/2;
					loc={left:l+"px",top:t+"px"};
					break;
				default://默认为右下角
					loc={right:"0px",bottom:"0px"};
					break;
			}
		}else{
			loc=location;
		}
		$(this).css("z-index","9999").css(loc).css("position","fixed");
		if(isIE6){
			if(loc.right!=undefined){
				//2008-4-1修改：当自定义right位置时无效，这里加上一个判断
				//有值时就不设置，无值时要加18px已修正层位置
				if($(this).css("right")==null || $(this).css("right")==""){
					$(this).css("right","18px");
				}
			}
			$(this).css("position","absolute");
		}
	});
};