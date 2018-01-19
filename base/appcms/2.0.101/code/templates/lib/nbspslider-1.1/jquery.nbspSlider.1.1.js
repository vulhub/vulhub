(function($) {
	$.fn.nbspSlider = function(options){
		var defaults = {
			widths:         '960px',        // 幻灯片宽度
			heights:        '300px',		// 幻灯片高度
			autoplay:       1,              // 是否自动播放幻灯片(1为是0为否)
			delays:         2000,           // 自动播放--间隔(毫秒)
			prevId: 		'prevBtn',      // 上一张幻灯片按钮ID
			nextId: 		'nextBtn',		// 下一张幻灯片按钮ID
			effect:	        'horizontal',   // horizontal、vertical、fade、none == 特效：横向、竖向、淡出、无特效
			speeds: 		1000,			// 幻灯片切换的速度(毫秒)
			altOpa:         0.5,            // ALT区块透明度
			altBgColor:     '#ccc',  		// ALT区块背景颜色
			altHeight:      '40px',  		// ALT区块高度
			altShow:         0,				// ALT区块是否显示(1为是0为否)
			altFontColor:    '#fff',        // ALT区块内的字体颜色
			altAlign:				'center', //ALT区块内文字对齐方式
			starEndNoEff: 	  0,            // 开始与结束中间无动画效果(1为是0为否)
			preNexBtnShow:    1,            // 是否显示上一张下一张按钮
			numBtnSty:        'num',	    // num、square、circle、roundness、rectangle == 数字、正方形、圆圈、圆形、长方形
			numBtnShow:       1             // 是否显示数字按钮
		}; 
		var options = $.extend(defaults, options);
		options.widths = parseInt(options.widths)+"px";
		options.heights = parseInt(options.heights)+"px";
		return this.each(function() {  
			var obj = $(this);            				
			var len = $("li", obj).length; 
			var end_index = len-1;		
			var start_index = 0;
			options.effect == 'horizontal' ? $(obj).find("ul")
			.css('width',(len*parseInt(options.widths))+'px')
			: $("ul",obj).css('width',options.widths);
			$(obj).css({
				width:options.widths,
				height:options.heights,
				position:'relative',
				overflow:'hidden'
			});
			
			$("ul",obj).css({
				position:'absolute',
				left:'0',
				overflow:'hidden'
			});
			
			$("ul li,img",obj).css({
				width:options.widths,
				height:options.heights,
				border:'none',
				float:'left'
			});
			
			$(obj).append(
				'<span class="sliderBtn" id='+options.prevId+'></span>'
				+'<span class="sliderBtn" id='+options.nextId+'></span>'
				+'<div id="altbox">'+$(obj).find("ul li img").eq(0).attr("alt")
				+'</div><div id="btnList"></div>'
			);
			
			$("#altbox",obj).css({
				'width':(parseInt(options.widths)-20)+'px',
				'height':options.altHeight,
				'line-height':options.altHeight,
				'opacity':options.altOpa,
				'background':options.altBgColor,
				'padding':'0 10px 0 10px',
				'position':'absolute',
				'bottom':'0',
				'color':options.altFontColor,
				'text-align':options.altAlign,
				'left':'0'
			});
			
			var $btnlens = len;
			for(var $i=1; $i<=$btnlens; $btnlens--){
				$(obj).find("#btnList").append("<li>"+(options.numBtnSty == "num" 
				? $btnlens : "<i style='float:left;text-indent:-9999px;'>"+$btnlens+"</i>")+"</li>");
			}
			
			$("#btnList",obj).css({
				'width':(parseInt(options.widths)-20)+'px',
				'left':'0'
			});
			
			if(options.numBtnSty != ""){ $("#btnList li",obj).addClass(options.numBtnSty);}
			
			$(".sliderBtn",obj).css("opacity",0.6).hover(function(){
				$(this).stop(true,false).animate({"opacity":"0.9"},300);	
			},function(){
				$(this).stop(true,false).animate({"opacity":"0.6"},300);	
			});
			
			options.preNexBtnShow == 0 ? $(".sliderBtn",obj).hide() : '';
 			options.altShow == 0 ? $(obj).find("#altbox").hide() : '';
			options.numBtnShow == 0 ? $("#btnList",obj).hide() : '';
			
			function trigger_eff(attr,fn_p,ActWhere){
				var p;
				fn_p ? p = fn_p : p = animates(attr,ActWhere);
				$("ul",obj).trigger("fn_"+options.effect,[p]);
			}
			
			$(".sliderBtn",obj).click(function(){
				var attr_result = $(this).attr("id").substring(0,4);
				trigger_eff(attr_result);
			});

			var $btn = $(obj).find("#btnList li");
			(function(btn){
				btn.hover(function(){
					(!$(this).hasClass(options.numBtnSty+"_act")) ? $(this).attr("class",options.numBtnSty+"_hover") : '';
				},function(){
					(!$(this).hasClass(options.numBtnSty+"_act")) ? $(this).attr("class",options.numBtnSty) : '';		
				});
			
			})($btn);
			
			function defBtnBgColor(){
				$btn.attr("class",options.numBtnSty);
			}
			defBtnBgColor
			
			$("#btnList li:last-child",obj).attr('class',options.numBtnSty+'_act');
			var spaces;
			$btn.click(function(){
				defBtnBgColor();
				start_index = (start_index > end_index) ? 0 : parseInt($(this).text())-1;
				var spaces = (function(){
					return options.effect == 'horizontal' ? start_index * parseInt(options.widths) 
					: start_index * parseInt(options.heights);
				})();
				
				spaces == 0 ? spaces : spaces = "-"+spaces;
				trigger_eff("next",parseInt(spaces)+"px",1);
				$(this).attr('class',options.numBtnSty+"_act");
			});
			
			function actFun(ranking){
				defBtnBgColor();
				$("#btnList li",obj).eq(ranking).attr('class',options.numBtnSty+"_act");
			}
			
			var animates = function(attr,ActWhere){
				attr == "next" ? (start_index = (start_index >= end_index) ? 0 : start_index+1) : 0;
				attr == "prev" ? (start_index = (start_index <= 0) ? end_index : start_index-1) : 0;
				ActWhere == 1 ? start_index = 0 : actFun("-"+(start_index+1));
				AltFun(start_index);
				var w_space = (start_index * parseInt(options.widths)*-1); 
				var h_space = (start_index * parseInt(options.heights)*-1);
				return options.effect == "horizontal" ? w_space : h_space;
			}
			
			$("ul",obj).bind("fn_"+options.effect,function(event,extent){
				options.effect == "horizontal" 
					? options.noEffect == 1 
						? $(this).css("margin-left",extent) 
						: ((start_index == 0 || start_index == end_index) && options.starEndNoEff == 1 ) 
							? $(this).css("margin-left",extent) 
							: $(this).stop().animate({marginLeft:extent},options.speeds) 
								: (options.effect == "none" || (options.starEndNoEff == 1 
								&& (start_index == 0 || start_index == end_index))) 
									? $(this).css("margin-top",extent) : options.effect == "fade" 
										? $(this).hide(1).animate({marginTop:extent},30).fadeIn("slow") 
										: $(this).animate({marginTop:extent},options.speeds);
				AltFun(start_index);
			});
			var t_width=options.widths.replace('px','');
			var t_height=options.heights.replace('px','');
			if(options.autoplay == 1){
        		var picTimer;
				$(obj).hover(function(){
					clearInterval(picTimer);
					//if(t_width>500 && t_height>100) $(".sliderBtn",obj).show();
				},function(){
					//if(t_width>500 && t_height>100) $(".sliderBtn",obj).hide();
					picTimer = setInterval(function(){
						trigger_eff("next");
					},options.delays); 
				}).trigger("mouseleave");
			}
			
			function AltFun(index){
				var alt = $(obj).find("ul li img").eq(index).attr('alt');
				$(obj).find("#altbox").text(alt);	
			}
		});
	};
})(jQuery);
