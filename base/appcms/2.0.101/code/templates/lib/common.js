/*
    Copyright (c) 2012-2013, loyjers@qq.com. All rights reserved.
    全局类：常用方法
*/

var C={
    /*
      半透明提示层
      支持：1）直接创建提示；2）页面内隐藏层变半透明显示
      width 提示层宽度
      height 提示层高度
      title 提示层标题内容
      content 提示层 提示内容
      timeout 友情提示 （错误提示） 显示时间， top 友情提示 上边距
      alert_style 设置 错误消息提示样式
      border 设置弹窗层边框粗细
      border_radius 设置边框圆角幅度
    */
    "alert":{
        "params":{
          "width":"300","height":"100","title":"温馨提示","content":"","timeout":"1000","top":"9","alert_style":{'border':'1px solid #ddd','color':'red'},'border':'8','border_radius':'5',
            //如果是取某个隐藏层内容显示到这里，则传入此标签
            "div_tag":""
        },
        "init":function(){
            C.alert.params={
              "width": "300", "height": "100", "title": "温馨提示", "content": "", "timeout": "1000", "top": "9", "alert_style": { 'border': '1px solid #ddd', 'color': 'red' }, 'border': '8', 'border_radius': '5',
            //如果是取某个隐藏层内容显示到这里，则传入此标签
            "div_tag":""};
        },
        //设置参数
        "set_params":function(params){
            for(var param in params){
                C.alert.params[param]=params[param];
            }
        },
        /*
            提示层，content 提示内容，支持HTML；top 顶边距 ;timeout 延迟关闭时间;
            必填：C.alert.tips({'content':'删除成功'});
            选填：C.alert.tips({'content':'删除成功','width':100,'top':100,'timeout':3000,'alert_style':{'border':'1px solid #000','color':'blue'}}); 参考C.alert.params 内部参数
        */
        "tips":function(params){
            if($('#tips_20130528').length>0) {return;}//存在则返回
            if(arguments[0]){C.alert.set_params(params);}
            var height=C.alert.params.height;//高度
            var content=C.alert.params.content;//内容
            var top=C.alert.params.top;//顶边距
            var timeout=C.alert.params.timeout;//延迟时间，毫秒
            var alert_style=C.alert.params.alert_style;//设置消息提示样式
            var tips_div_opacity='<div id="tips_20130528_opacity" style="width:100%;height:100%;position:absolute;background:#fff;left:0;top:0;z-index:19000;filter:Alpha(opacity=10);opacity:0.1;"></div>';
            var ntop=parseInt($(document).scrollTop())+parseInt(top);//alert(ntop);
            var tips_div='<div id="tips_20130528" style="-moz-border-radius:5px;-webkit-border-radius:5px;border-radius:5px;background:#fffbeb;padding:8px 40px 8px 40px;position:absolute;z-index:20000;top:'+ntop+'px;'+'">'+content+'</div>';
            $('body').append(tips_div_opacity+tips_div);//动态创建层
            parseInt(params.width) > 0 ? $('#tips_20130528').css('width',params.width+'px') : $('#tips_20130528').css('width','auto'); //设置宽度
            params.alert_style==undefined ? $('#tips_20130528').css(alert_style) : $('#tips_20130528').css(params.alert_style) ;//设置消息提示样式
            $('#tips_20130528').css({'left':(($(document).width()-$('#tips_20130528').width())/2-40)+'px'});//设置层显示左右居中
            setTimeout(function(){$('#tips_20130528_opacity').remove();$('#tips_20130528').remove();},timeout);//延时移除层
            C.alert.init();
        },

        //显示透明层提示
        "opacty":function(params){
            if(arguments[0]){
                C.alert.set_params(params);
            }
            //内部参数
            var width=C.alert.params.width;
            var height=C.alert.params.height;
            var title=C.alert.params.title;
            var content=C.alert.params.content;
            var div_tag=C.alert.params.div_tag;
            var border = C.alert.params.border;
            var border_radius = C.alert.params.border_radius;
            var border_style = params.border != undefined ? params.border : border; //判断是否传入 border
            var border_radius_style = params.border_radius != undefined ? params.border_radius : border_radius; //判断是否传入 border_radius
            //计算位置
            var w=$(document).width();
            var h=$(window).height();
            var h_doc=$(document).height();
            var h_scroll = $(document).scrollTop();
            var opacty_width = $(window).width();
          //样式
            class_opacty={'display':'block','position':'absolute','width': opacty_width +'px','height':h_doc - 4+'px','background':'#000','top':'0px','left':'0px','filter':'Alpha(opacity=20)','opacity':'0.3'};//透明层
            class_tips = { 'position': 'absolute', 'left': ((w - width) / 2 - 8) + 'px', 'top': ((h - height) / 2 + h_scroll - 8) + 'px', 'background': '#fff', 'width': width + 'px', 'height': height + 'px', border: '' + border_style + 'px solid #555' };//提示层
            class_tit = { 'cursor': 'move', 'height': '30px', 'background': '#ddd' };//标题栏
            class_tit_left={'font-weight':'bold','float':'left','margin-left':'8px','_margin-right':'3px','line-height':'30px'};//标题栏左侧
            class_tit_right={'float':'right','cursor':'pointer'};//标题栏右侧
            //透明层HTML
            var div_opacty='<div class="opacty" style="z-index:9999;"></div>';
            //提示层标题栏HTML
            var tips_tit = '<div class="tips_tit"><div class="drag-tit"><span class="tit_left">' + title + '</span></div><span class="tit_right" onclick="C.alert.opacty_close(\'' + div_tag + '\');"><a style="font-size:18px;color:#888;font-weight:600;display:inline-block;width:40px; height30px; line-height:28px; text-align:center;" class="close_a" title="关闭">x</a></span><span style="clear:both;"></span></div>';
			if ($(".opacty").length > 0) { return false; }
			
             //1)如果传入URL
            if(params.url != undefined ){
                $('body').append(div_opacty);//插入透明层
                $('.opacty').css(class_opacty);
                var div_tips = '<div class="div_tips" style="overflow:hidden;-moz-border-radius:' + border_radius_style + 'px;-webkit-border-radius:' + border_radius_style + 'px;border-radius:' + border_radius_style + 'px;">' + tips_tit + '<iframe name="iframe_info" scrolling="no" class="iframe_info" src="' + params.url + '" ></iframe></div>'; //内容
                $('body').append(div_tips);
                var iframe_style = {'overflow':'hidden','margin':'0','padding':'0','border':'0','width':'100%','height':height - $('.tips_tit').height() - 16 + 'px'};//iframe 元素样式
                var opacty_zindex=10001;
                $('.div_tips').css({'z-index':(opacty_zindex+3),'overflow':'hidden'});
                $('.tips_tit').css(class_tit);$('.tit_left').css(class_tit_left);$('.tit_right').css(class_tit_right);
                $('.div_tips').css(class_tips);//内容层 插入样式
                $('.div_tips').css('z-index','10099'); //内容显示深度
                $('.iframe_info').css(iframe_style); //iframe 插入样式
                drag_disCenter();
                return false;
            }
            //2）如果传入的是层标签
            if(div_tag!=''){
                //层外部前面插入透明层
                $(div_opacty).insertBefore(div_tag);
                var opacty_zindex=10000;
                //层内容插入标题栏
                $(div_tag).prepend(tips_tit);
                $('.opacty').css(class_opacty);
                $('.tips_tit').css(class_tit);$('.tit_left').css(class_tit_left);$('.tit_right').css(class_tit_right);
                //重定义层位置和样式
                $(div_tag).css({ 'display': 'block', 'z-index': (opacty_zindex + 10), 'position': 'absolute', 'left': ((w - width) / 2 - 8) + 'px', 'top': ((h - height) / 2 + h_scroll - 8) + 'px', 'background': '#fff', 'width': width + 'px', 'height': height + 'px', border: '' + border_style + 'px solid #555', '-moz-border-radius': +border_radius_style + 'px', '-webkit-border-radius': +border_radius_style + 'px', 'border-radius': +border_radius_style + 'px' });
                $(".close_a").hover(function () { $(this).css({ 'color': '#e2041b', 'text-decoration': 'none' }) }, function () { $(this).css({ 'color': '#888' }) }); //设置A 连接样式
                if ($('#addrecommend_html').length > 0) { $('#addrecommend_html').css({'overflow-x':'hidden'}); } //调用 编辑器时内容溢出，去除内容溢出
                drag_disCenter(div_tag);
                return false;
            }
            //3）如果只是显示传入的内容
            //创建透明层
            if(params.content != undefined){
                $('body').append(div_opacty);
                $('.opacty').css(class_opacty);
                //创建提示层
                var div_tips = '<div class="div_tips" style="-moz-border-radius:' + border_radius_style + 'px;-webkit-border-radius:' + border_radius_style + 'px;border-radius:' + border_radius_style + 'px;">' + tips_tit + '<div class="tips_content"><table cellspacing="0" cellpadding="0" width="100%" height="60%" border="0"><tr><td align="center">' + content + '</td></tr></table><div></div>';
                $('body').append(div_tips);
                var opacty_zindex=10001;
                $('.div_tips').css(class_tips);
                $('.div_tips').css({'z-index':(opacty_zindex+3)});
                $('.tips_tit').css(class_tit);$('.tit_left').css(class_tit_left);$('.tit_right').css(class_tit_right);
                $('.tips_content').css({ 'line-height': '170%', 'text-align': 'center' });
                drag_disCenter();
            }

            //调用 拖拽及弹出层居中显示方法
            function drag_disCenter(div_tag){
              if(!arguments[0]) div_tag='.div_tips';
              C.alert.display_center(div_tag); //弹出层缩放滚动同步
              C.alert.display_opacty('.op');//透明层自增长
			  $('.drag-tit').css({'width': $('.tips_tit').width() - $('.tit_right').width() ,'height': $('.tips_tit').height(),'float':'left'}); //拖拽条样式
              C.alert.draw_alert(div_tag, '.drag-tit'); //拖拽
              $(".close_a").hover(function () { $(this).css({ 'color': '#e2041b', 'text-decoration': 'none' }) }, function () { $(this).css({ 'color': '#888' }) }); //设置A 连接样式
              $('.tb3').css({'margin':'5px 0 0 10px'});
              //实现ESC键关闭弹出层
			  $(window).on('keydown',function(event){
				 if(event.keyCode== 27 && $('.opacty').is(':visible')){C.alert.opacty_close(C.alert.params.div_tag);}
			  });
           }
        },
        /*
         拖拽
         obj 移动容器，title 标题位置为拖动位置
        */
        "draw_alert": function (Drag, Title) {
            if(!arguments[1]) Title='.drag-tit';
            var objDarg = $(Drag);
            var objTitle = $(Title);
            var posX = posY = 0;
            var _move = false; //移动状态
            var opacity;
            objTitle.mousedown(function (e) {
              _move = true;
              opacity = 0.2;
              posX = e.pageX - $(this).offset().left; //获取鼠标坐标
              posY = e.pageY - $(this).offset().top;
            });
          $(document).mousemove(function (e) {
              var e = e || window.event;
              var maxW = $(window).width() - objDarg.get(0).offsetWidth; //可移动最大宽度
              var maxH = $(window).height() - objDarg.get(0).offsetHeight - 10;
              if (_move) {
                var _x = e.pageX - posX;
                var _y = e.pageY - posY;
                if (_x < 0) _x = 0; else if (_x > maxW) _x = maxW;
                if (_y < 0) _y = 0; else if (_y > maxH) _y = maxH;
              }
              objDarg.css({ left: _x, top: _y, 'opacity': opacity });
            }).mouseup(function () {
              _move = false;
              opacity = 1;
            });
          },
        /*
         始终显示在屏幕中间 obj:始终居中的弹出层对象
        */
          "display_center": function (obj) {
            if (arguments[0]) alert_obj = '.div_tips';
            //弹出层居中
            function setMask() {
              var top = parseInt(($(window).scrollTop() + ($(window).height() - $(obj).height()) / 2)) + "px";
              var left = parseInt(($(window).scrollLeft() + ($(window).width() - $(obj).width()) / 2)) + "px";
              $(obj).css({ top: top, left: left });
            }
            //缩放滚动同步
            $(window).bind('resize', setMask);
            $(window).bind('scroll', setMask);
          }
          ,
          /*
            opacty_obj 透明层自增长（高度）
          */
          "display_opacty":function (opacty_obj) {
            if (arguments[0]) opacty_obj = '.opacty';
            //透明层 自增长高度
            function set_opacty_mask() {
              var opacty_height = $(window).height() + $(window).scrollTop();
              $(opacty_obj).height(opacty_height);
            }
            //透明层缩放滚动同步
            $(window).bind('resize', set_opacty_mask);
            $(window).bind('scroll', set_opacty_mask);
         }
        ,
        //关闭透明层提示
        "opacty_close":function(div_tag){
			if(!arguments[0]) div_tag = '';
            //删除透明层
            $('.opacty').remove();
            if(div_tag==''){
                //删除提示层
                $('.div_tips').remove();
            }else{
                //隐藏提示层
                $('.tips_tit').remove();
                $(div_tag).css({'display':'none'});
            }
            C.alert.init();
        }
    },
    //cookie读写
    "cookie":{
        //设置Cookie
        "set":function(name,value,hours){
            var __hours=1;
            if(arguments[2]){__hours=hours;}
            var exp = new Date();exp.setTime(exp.getTime() + __hours*60*60*1000);
            document.cookie = name + "="+ escape (value) + ";expires=" + exp.toGMTString();
        },
        //读取Cookie
        "get":function(name){
            var arr,reg=new RegExp("(^| )"+name+"=([^;]*)(;|$)");
            if(arr=document.cookie.match(reg)){return unescape(arr[2]);}else{return null;}
        }
    },
    //加入收藏夹
    "fav":function(tit,url){
        try{
            window.external.addFavorite(url, tit);
        }catch(e){
            try{
                window.sidebar.addPanel(tit,url,'');
            }catch(e){
                alert('您可以尝试通过快捷键' + ctrl + ' + D 加入到收藏夹~');
            }
        }
    },
    //设为首页
    "sethome":function(url){
        if (document.all){
            document.body.style.behavior='url(#default#homepage)';
            document.body.setHomePage(url);
        }else if (window.sidebar){
            if(window.netscape){
                try{
                    netscape.security.PrivilegeManager.enablePrivilege("UniversalXPConnect");
                }catch (e){
                    alert( "该操作被浏览器拒绝，如果想启用该功能，请在地址栏内输入 about:config,然后将项 signed.applets.codebase_principal_support 值该为true" );
                }
            }
            //if(window.confirm("你确定要设置"+url+"为首页吗？")==1){
                var prefs = Components.classes['@mozilla.org/preferences-service;1'].getService(Components.interfaces.nsIPrefBranch);
                prefs.setCharPref('browser.startup.homepage',url);
            //}
        }
    },
    //日期时间
    "date":{
        "localtime":function(nS){
            var dstr=new Date(parseInt(nS) * 1000).toLocaleString().replace(/:\d{1,2}$/,' ');
            dstr=dstr.replace('年','-').replace('月','-').replace('日','');
            return dstr;
        }
        ,"new_date":function(str){
            str=str.replace(/(\/)|(年)|(月)/g,'-');
            str=str.replace(/(日)/g,'');str=str.split(' ')[0];
            var newstr = str.split('-');
            var date = new Date();
            date.setUTCFullYear(newstr[0], newstr[1] - 1, newstr[2]);
            date.setUTCHours(0, 0, 0, 0);
            return date;
        }
    },
    //字符串处理
    "string":{
        //字符串换行清除
        "clear_line":function(str){
            return str.replace(/\n/g,'').replace(/\r/g,'').replace(/\t/g,'');
        }
        //转移SQL中的单撇号
        ,"sql_filter":function(str){
            return str.replace(/(\')/g,"''");
        }
        //判断字符串长度
        ,"length":function(s){
            var char_length = 0;
            for (var i = 0; i < s.length; i++){
                var son_char = s.charAt(i);
                if(encodeURI(son_char).length > 3){
                    char_length += 2;
                }else{
                    char_length += 1;
                }
            }
            return char_length;
        }
        //判断是否标点符号
        ,"bd_char":function(str){
            if(str.indexOf(',')>=0) return 1;
            if(str.indexOf('，')>=0) return 1;
            return 0;
        }
        //替换逗号
        ,"repair_char":function(m){
            var str=$(m).val();
            $(m).val(str.replace(/(，)|(\|)/g,','));
        }
        //全角数字转半角
        ,"dsbc":function(str,flag) {
            var i;
            var result='';
            if (str.length<=0) {return '';}
            for(i=0;i<str.length;i++)
            { str1=str.charCodeAt(i);
            if(str1<125&&!flag)
            result+=String.fromCharCode(str.charCodeAt(i)+65248);
            else
            result+=String.fromCharCode(str.charCodeAt(i)-65248);
            }
            return result;
        }
    },
    /*TAB按钮页，可多重嵌套无限使用
        tabs 方法参数说明：JSON格式
        {
            "selected":"#n3" // 选中的选项卡，没有此参数默认选中第一个
            ,"event":"mouseover" //切换触发动作 click，mouseover
            ,"style":{        //选项卡样式
                "sclass":"selected"    //选中
                ,"nclass":"noselected" //未选中
            }
            ,"params": //选项卡组
            [
                {"nav":"#n1","con":"#c1","sclass":"selected2","nclass":"noselecte2"}, //nav：选项卡ID，con：选项卡对应内容层ID，sclass,nclass，自定义样样式，没有则默认为上级的 style参数中定义
                {"nav":"#n2","con":"#c2"},
                {"nav":"#n3","con":"#c3"}
            ]
        }
        简写方式：C.tabs({"params":[{"nav":"#n1","con":"#c1"},{"nav":"#n2","con":"#c2"},{"nav":"#n3","con":"#c3"}]});
    */
    "tabs":function(__params){
        //默认选中
        var selected=__params.selected;
        if(__params.selected){selected=__params.selected}else{selected=__params.params[0].nav;}
        //切换动作
        var event=__params.event;
        if(__params.event){event=__params.event}else{event='click';}
        //默认样式选中和不选中
        if(!__params.style) __params.style={"sclass":"selected","nclass":"selected_no"};
        //切换卡参数
        var params=__params.params;
        //遍历切换卡参数
        for(var i=0;i<params.length;i++){
            var tab=params[i];
            //选项卡自定义了样式
            var sclass=__params.style.sclass;if(tab.sclass) sclass=tab.sclass;
            var nclass=__params.style.nclass;if(tab.nclass) nclass=tab.nclass;
            //判断选中选项卡
            if(selected==tab.nav){
                $(tab.nav).removeClass(nclass);
                $(tab.nav).addClass(sclass);
                $(tab.con).css({'display':''});
            }else{
                $(tab.nav).removeClass(sclass);
                $(tab.nav).addClass(nclass);
                $(tab.con).css({'display':'none'});
            }//alert(event);
            //绑定事件
            $(tab.nav).unbind(event);
            $(tab.nav).bind(event,function(){
                C.tabs({"selected":"#"+$(this).attr('id'),"event":event,"style":__params.style,"params":params});
            });
        }
    },
    /*lib库中nbslide幻灯片组件增加功能，使用增加缩略图预览功能
        在幻灯片下方循环输出幻灯广告缩略图 thumb，每个图片的元素加上 onclick="C.slide_click(obj,num)"
        obj 为幻灯片容器标识如 #abc,.abc，num为图片序号，0,1,2,....，反序的
    */
    "nbslide":{
        //缩略图点击显示对应幻灯
        "thumb_click":function(obj,num){
            var btn=$(obj).find("#btnList li");
            btn.each(function(index){
                if(index==num){
                    $(this).click();
                }
            });
        },
        //左右方向按钮控制上一个下一个幻灯
        "pn_click":function(obj,pn){
            var btn=$(obj).find(".sliderBtn");
            if(pn=='prev') $(obj).find("#prevBtn").click();
            if(pn=='next') $(obj).find("#nextBtn").click();
        }
    },
    /*
     *  创建上传按钮
     *  params{
     *      "inner_box":"#span_id"   传了该参数则把上传iframe嵌入该ID表示的元素内部，否则在上面地方调用则直接 document.write
     *  }
     */
    "create_upload_iframe":function(params){
        //alert(upload_path);
        var a=$.evalJSON(params);
        a['domain']=document.domain;
        params=$.toJSON(a);
        var frame_code='<iframe style="" src=\''+upload_path+'upload/upload_form.php?params='+encodeURIComponent(params)+'\' width="80" height="28" frameborder="no" border="0″ marginwidth="0″ marginheight="0" scrolling="no" allowtransparency="yes"></iframe>';
        if(a.inner_box){
            $(a.inner_box).html(frame_code);
        }else{
            document.write(frame_code);
        }
    },
    //ckeditor 编辑器贴片上传按钮并把返回值插入编辑器
    "ckeditor":{
        //编辑器的上传回调处理
        "callback":function(ret){
            try{
                var json=$.evalJSON(ret);//alert(ret);
                var editor=eval('('+[json.params.editor]+')');//转换字符串为编辑器变量名
                //没有取到回传文件，上传失败
                if(json.files.length<=0){
                    alert('上传失败');
                    return;
                }
                //图片
                if(json.files[0].original.match(/(\.gif)|(\.png)|(\.jpg)|(\.jpeg)|(\.bmp)/i)){
                    editor.insertHtml('<img src="'+json.files[0].original+'">');
                //flash
                }else if(json.files[0].original.match(/(\.swf)/)){
                    var code='<object classid="clsid:d27cdb6e-ae6d-11cf-96b8-444553540000" codebase="http://fp'+
                            'download.macromedia.com/pub/shockwave/cabs/flash/swflash.cab#version=8,0,0,0" width="400" height="300" id="flashvars" align="center">'+
                            '<param name="allowscriptAccess" value="sameDomain" />'+
                            '<param name="movie" value="'+json.files[0].original+'" />'+
                            '<param name="quality" value="high" /><param name="bgcolor" value="#ffffff" />'+
                            '<embed quality="high" bgcolor="#ffffff" width="400" src="'+json.files[0].original+'" height="300" allowScriptAccess="never" allowNetworking="internal" autostart="0" name="flashvars" align="center" type="application/x-shockwave-flash" pluginspage="http://www.macromedia.com/go/getflashplayer" />'+
                            '</object>';
                    editor.insertHtml(code);
                //其他类型的文件
                }else{
                    var size=(json.files[0].size/1000);
                    size=size.toString().replace(/\.\d+/,'');
                    var code='<div style="background:#F2EEDD;padding:5px 10px;"><a href="'+json.files[0].original+'" target=_blank>'+json.files[0].original+'</a> <span>文件大小：'+size+' kb</span> <a href="'+json.files[0].original+'" target="_blank">点击下载附件</a> </div>';
                    //alert(code);
                    //$("#test_inner").html(code);
                    editor.insertHtml(code);
                }
            }catch(e){
                alert('err:'+e.message);
            }
        },
        /*
         *  CKeditor编辑器上传按钮
         *  upload_btn 按钮外部层
         *  editor  CKeditor编辑器变量名,不允许以editor为变量名
         *  注意：ckeditor 的JS上两级元素div,td要 position:relative;
         *  //使用范例 <span id="upbtn_box2"><script>C.ckeditor.init('#upbtn_box2','editor2');</script></span>
         */
        "init":function(upload_btn,editor){
            if(editor=='editor'){alert('编辑器变量不允许命名为 editor');return}
            C.create_upload_iframe('{"func":"C.ckeditor.callback","inner_box":"'+upload_btn+'","editor":"'+editor+'","thumb":{"width":"300","height":"300"}}');
        }
    },

    /*表单操作方法
        params=["#a",".b",["#abc","abc"]] 数组中有两种类型，字符串和对象
        字符串统一赋空值或者默认第一个的值（select,checkbox,radio）
        对象则赋指定的值
    */
    "form": {
      //新表单初始化或者给表单赋默认值
      "init": function (params) {
        //遍历需要初始化的表单
        for (var i = 0; i < params.length; i++) {
          var pname = params[i];
          //判断文本还是对象
          if (typeof (pname) == 'string') {
            //类初始化
            if (pname.substr(0, 1) == '.') {
              $(pname).each(function (index) {
                C.form.setval($(this));
              });
            }
            //id初始化
            if (pname.substr(0, 1) == '#') {
              C.form.setval($(pname));
            }
          } else if (typeof (pname) == 'object') {
            //符合数组格式
            if (pname.length == 2) {//alert(pname[0]);
              //类初始化
              if (pname[0].substr(0, 1) == '.') {
                $(pname[0]).each(function (index) {
                  C.form.setval($(this), pname[1]);
                });
              }
              //id初始化
              if (pname[0].substr(0, 1) == '#') {
                C.form.setval($(pname[0]), pname[1]);
              }
            }
          }
        }
      },
      /*
       *获取表单 元素值
       *通过formID 获取该form 下所有表单元素值
       */
      "get_form": function (formID) {
        var value = [];
        formID = formID.indexOf('#') >= 0 ? formID : '#' + formID + ''; //判断参数是否带有#

        $(formID).find('input').each(function (index) {
          var inp_type = this.type;
          if (inp_type == 'text' || inp_type == 'hidden') {
            value.push('"' + this.id + '":"' + encodeURIComponent(this.value) + '"');
          }
          if (inp_type == 'radio' || inp_type == 'checkbox') {
            if ($(this).prop('checked')) {
                value.push('"' + this.name + '":"' + encodeURIComponent($("input[name='"+this.name+"']:checked").val()) + '"');
            }
          }
        });
        $(formID).find('select').each(function () {//下拉
          value.push('"' + this.id + '":"' + encodeURIComponent($(this).find("option:selected").val()) + '"');
        });
        $(formID).find('textarea').each(function () {//多行文本
          value.push('"' + this.id + '":"' + encodeURIComponent($(this).val()) + '"');
        });
        //alert("{" + value.join(',') + "}");
        return $.evalJSON("{" + value.join(',') + "}");//转换json格式
      },
      /*
       *通过formID 赋值 给该form 下ID对应的表单元素
       *data 数据源
       */
      "set_form": function (formID, data) {
        formID = formID.indexOf('#') >= 0 ? formID : '#' + formID + ''; //判断参数是否带有#
        var value = [];
        for (var getID in data) {
          value.push(['#' + getID, data[getID]]);
        }
        C.form.init(value);
      },
      //给表单设值，自动判断类型
      "setval": function (obj, str) {
        if (!arguments[1]) var str = '';
        //文本框，密码框，直接赋值
        if (obj.is('input') && (obj.attr('type') == 'text' || obj.attr('type') == 'password' || obj.attr('type') == 'hidden')) {
          obj.val(str);
        }
        //单选框，复选框
        if (obj.is('input') && (obj.attr('type') == 'radio' || obj.attr('type') == 'checkbox')) {
          if (obj.attr('value') == str) obj.attr('checked', 'true');
        }
        //下拉框，多行文本框
        if (obj.is('select') || obj.is('textarea')) {
          obj.val(str);
        }

      },
      /* 根据主键值修改某个值的公用方法
          url 处理地址
          tag_class input 的class类
          tag_id  input 的表的主键
          order_update('/admin_user.php','order','pid');
          input 格式 <input type="text" class="order" pid="<?php echo();?>">
      */
      "update_field": function (url, tag_class, tag_id) {
        if (!arguments[0]) { C.alert.tips({ "content": "错误：没有处理程序地址" }); return; }
        if (!arguments[1]) tag_class = '.order';
        if (!arguments[2]) tag_id = 'pid';
        var params = [];
        //遍历所有input 值
        $(tag_class).each(function () {
          var id = $(this).attr(tag_id);
          var val = $(this).val();
          params.push({ "id": id, "val": val });
        });
        if (params.length == 0) { C.alert.tips({ "content": "没有需要修改排序的数据" }); return; }
        //转换为字符串并URL编码
        var params_str = $.toJSON(params);
        //params_str=encodeURI(params_str);

        //提交数据处理
        $.post(url, { params: params_str }, function (result) {
          try {
            var ret = $.evalJSON(result);
            if (ret.code == 0) {
              C.alert.tips({ "content": "" + ret.msg + "" });
              setTimeout(function () { window.location.reload(); }, 1200);
            } else {
              C.alert.tips({ "content": "" + ret.msg + "" });
            }
          } catch (e) {
            alert(result + e);
          }
        });
      },
      /* 根据主键值删除批量数据，或者批量修改某个字段为同一个值，根据CHECKBOX选中 公用方法
          url 处理地址
          tag_class checkbox 的class类
          checkbox_value('/admin_user.php','.pid');
          checkbox 格式 <input type="checkbox" id="a61" class="ipt" value="1">
      */
      "batch_modify": function (url, tag_class) {
        if (!arguments[0]) { C.alert.tips({ "content": "错误：没有处理程序地址" }); return; }
        if (!arguments[1]) { tag_class = '.ipt' }
        var params = [];
        //遍历所有checkbox值
        $(tag_class).each(function () {
          if ($(this).attr('checked') == 'checked') {
            params.push($(this).val());
          }
        });
        if (params.length == 0) { C.alert.tips({ "content": "没有选中项，无法操作" }); return; }
        var params_str = params.join(',');
        var url = url.replace('[@]', params_str);

        //提交数据处理
        $.post(url, { params: params_str }, function (result) {
          try {
            var ret = $.evalJSON(result);
            if (ret.code == 0) {
              C.alert.tips({ "content": "" + ret.msg + "" });
              setTimeout(function () { window.location.reload(); }, 1200);
            } else {
              C.alert.tips({ "content": "" + ret.msg + "" });
            }
          } catch (e) {
            alert(result + e);
          }
        });
      },
      /*获取页面中checkbox选中值
        inp_class checkbox class 名称
      */
      "get_checkbox_val":function (inp_class) {
        if ($(inp_class).length < 0) return;
        var value='';
        $(inp_class).each(function () {
          var inp_type = this.type;
          if (inp_type == "checkbox") {
            if ($(this).prop('checked')) {
              value += $(this).val()+',';
            }
          }
        });
        value = value.substring(0, value.lastIndexOf(','));
        return value;//转换json格式
      },
      //全选和反选,div_tag ，可以是 #a,.a这样的
      "check_all": function (div_tag) {
        $(div_tag).each(function () {
          if ($(this).attr('checked') == 'checked') { $(this).attr({ 'checked': false }); } else { $(this).attr({ 'checked': true }); }
        });
      },
      //输入框默认文本
      "input_default": function (input, value) {
        if ($(input).val() == '') $(input).val(value);
        $(input).mousedown(function () {
          if ($(this).val() == value) $(this).val('');
        }).blur(function () {
          if ($(this).val() == '') $(this).val(value);
        });
      },
      //tips ={"id":"content","id":"content","id":"content",...} //{"#id1":"请输入值","#id2":"请输入值"}
      "text_tips": function (tips) {
        if (tips.length < 0) return;
        for (var inp_id in tips) { //循环取值

          if (inp_id.indexOf('#') >= 0)  //判断传入ID 是否带有 #
            inp_id = inp_id.substring(inp_id.indexOf('#') + 1);

          var html = $('<label class="labRemind" for=' + inp_id + '>' + tips['#' + inp_id] + '</label>'); //标签初始化
          if (inp_id.indexOf('#') < 0) inp_id = '#' + inp_id + '';
          $(inp_id).parent().prepend(html); //插入标签
          var font_size = $(inp_id).css('font-size') == undefined ? 12 + 'px' : $(inp_id).css('font-size'); //获取 文本框字体大小
          $(inp_id).parent().css('position', 'relative'); //增加父元素 相对定位
          $('.labRemind').css({ 'font-size': font_size, 'color': '#ccc', 'position': 'absolute', 'height': $(inp_id).parent().height() + 'px', 'line-height': $(inp_id).parent().height() + 'px', 'float': 'left', 'cursor': 'text', 'text-indent': '6px' }); //标签样式
        }

        $('input').each(function () {//遍历当前页面所有input
          var lab = $(this).siblings('.labRemind');
          if (lab.length > 0) { //判断文本框同辈元素是否存在消息提示
            $(this).val() == '' ? lab.show() : lab.hide(); //判断文本框是否有值，无则显示提示，反之
            $(this).focus(function () { lab.hide(); })//获取焦点
              .blur(function () { //失去焦点
                $(this).val() != "" ? lab.hide() : lab.show();
              });
          }
        });
      },
      /*
       * 带行号的多行文本框
       */
      "textarea":{
        "params":{"name":"txt1","id":"txt1","width":"500px","height":"200px","content":"","line_height":"20"},
        "init":function(){
            C.form.textarea.params={"name":"txt1","id":"txt1","width":"500px","height":"200px","content":"","line_height":"20"};
        },
        //设置参数
        "set_params":function(params){
            for(var param in params){
                C.form.textarea.params[param]=params[param];
            }
        },
        "create_line":function(params){
            if(arguments[0]){C.form.textarea.set_params(params);}else{C.form.textarea.init();}
            var content=C.form.textarea.params.content;content=content.replace(/\+/g,' ');content=decodeURIComponent(content);
            var width=C.form.textarea.params.width;
            var height=C.form.textarea.params.height;
            var line_height=C.form.textarea.params.line_height;
            var name=C.form.textarea.params.name;
            var id=C.form.textarea.params.id;

            var css_textarea_con={"width":width,"height":height,"overflow-x":"hidden","overflow-y":"scroll","margin-top":"10px","border":"1px solid #ccc","border-width":"1px 0 1px 1px"};
            var css_textarea={"word-wrap":"normal","margin":"0","padding":"0","resize": "none","border":"0px","float":"left","overflow":"hidden","line-height":line_height+"px","font-size":"12px"};
            var css_txt_left={"word-wrap":"normal","margin":"0","padding":"0","resize": "none","width":"40px","background":"#ccc","height":height,'text-align':'center'};
            var html='';
            html+='<div class="textarea_con">';
            html+='<textarea class="txt_left"></textarea>';
            html+='<textarea name="'+name+'" id="'+id+'" class="txt_right">'+content+'</textarea>';
            html+='<div style="clear:both;height:5px;"></div>';
            html+='</div>';
            document.write(html);
            var obj_m=$('.textarea_con');
            var obj_l=$('.textarea_con > textarea.txt_left');
            var obj_r=$('.textarea_con > textarea.txt_right');
            obj_m.css(css_textarea_con);//容器样式
            $('.textarea_con > textarea').css(css_textarea);//容器中textarea统一样式
            obj_l.css(css_txt_left);//左侧宽度高
            obj_r.css({"width":(obj_m.width()-obj_l.width()-37)+'px',"height":height});//右侧宽高

            //初始化
            C.form.textarea.draw($(obj_r).val(),obj_m,obj_l,obj_r);
            //附加事件
            $(obj_r).keyup(function(){
                C.form.textarea.draw($(obj_r).val(),obj_m,obj_l,obj_r);
            });
        },
        "draw":function(content,obj_m,obj_l,obj_r){
            var line_height=C.form.textarea.params.line_height;
            var con_arr=content.split('\n');
            var con_l='';
            for(var i=0;i<con_arr.length;i++){
                con_l+=(i+1)+'\n';
            }
            obj_l.val(con_l);
            var con_height=line_height*con_arr.length;
            if(con_height<obj_m.height()){con_height=obj_m.height();}
            obj_l.css({"height":con_height+"px"});
            obj_r.css({"height":con_height+"px"});
        }
      }
    }
};



/*错误提交js*/
function setbug(appid,data_center_url){
    if(confirm('真的无法下载吗？')){
        $.getJSON(data_center_url+"/apperror.php?&jsoncallback=?",{"appid":appid},
        function(json){
            try{
                alert(json.msg);
            }catch(e){alert(e.message);}
        });
    }
 }

//jquery 扩展方法

//扩展jQuery对json字符串的转换,evalJSON,toJSON方法
jQuery.extend({
   /** * @see 将json字符串转换为对象 * @param json字符串 * @return 返回object,array,string等对象 */
   evalJSON: function(strJson) {
     return eval("(" + strJson + ")");
   },
   /** * @see 将javascript数据类型转换为json字符串 * @param 待转换对象,支持object,array,string,function,number,boolean,regexp * @return 返回json字符串 */
   toJSON: function(object) {
     var type = typeof object;
     if ('object' == type) {
       if (Array == object.constructor) type = 'array';
       else if (RegExp == object.constructor) type = 'regexp';
       else type = 'object';
     }
     switch (type) {
     case 'undefined':
     case 'unknown':
       return;
       break;
     case 'function':
     case 'boolean':
     case 'regexp':
       return object.toString();
       break;
     case 'number':
       return isFinite(object) ? object.toString() : 'null';
       break;
     case 'string':
       return '"' + object.replace(/(\\|\")/g, "\\$1").replace(/\n|\r|\t/g, function() {
         var a = arguments[0];
         return (a == '\n') ? '\\n': (a == '\r') ? '\\r': (a == '\t') ? '\\t': ""
       }) + '"';
       break;
     case 'object':
       if (object === null) return 'null';
       var results = [];
       for (var property in object) {
         var value = jQuery.toJSON(object[property]);
         if (value !== undefined) results.push(jQuery.toJSON(property) + ':' + value);
       }
       return '{' + results.join(',') + '}';
       break;
     case 'array':
       var results = [];
       for (var i = 0; i < object.length; i++) {
         var value = jQuery.toJSON(object[i]);
         if (value !== undefined) results.push(value);
       }
       return '[' + results.join(',') + ']';
       break;
     }
   }
});

//日期格式化
Date.prototype.format = function(format)
    {
        var o = {
        "M+" : this.getMonth()+1, //month
        "d+" : this.getDate(),    //day
        "h+" : this.getHours(),   //hour
        "m+" : this.getMinutes(), //minute
        "s+" : this.getSeconds(), //second
        "q+" : Math.floor((this.getMonth()+3)/3),  //quarter
        "S" : this.getMilliseconds() //millisecond
        }
        if(/(y+)/.test(format)) format=format.replace(RegExp.$1,
        (this.getFullYear()+"").substr(4 - RegExp.$1.length));
        for(var k in o)if(new RegExp("("+ k +")").test(format))
        format = format.replace(RegExp.$1,
        RegExp.$1.length==1 ? o[k] :
        ("00"+ o[k]).substr((""+ o[k]).length));
        return format;
    }

//COOKIE操作
jQuery.cookie = function(name, value, options) {
    if (typeof value != 'undefined') { // name and value given, set cookie
        options = options || {};
        if (value === null) {
            value = '';
            options.expires = -1;
        }
        var expires = '';
        if (options.expires && (typeof options.expires == 'number' || options.expires.toUTCString)) {
            var date;
            if (typeof options.expires == 'number') {
                date = new Date();
                date.setTime(date.getTime() + (options.expires * 24 * 60 * 60 * 1000));
            } else {
                date = options.expires;
            }
            expires = '; expires=' + date.toUTCString(); // use expires attribute, max-age is not supported by IE
        }
        var path = options.path ? '; path=' + options.path : '';
        var domain = options.domain ? '; domain=' + options.domain : '';
        var secure = options.secure ? '; secure' : '';
        document.cookie = [name, '=', encodeURIComponent(value), expires, path, domain, secure].join('');
    } else { // only name given, get cookie
        var cookieValue = null;
        if (document.cookie && document.cookie != '') {
            var cookies = document.cookie.split(';');
            for (var i = 0; i < cookies.length; i++) {
                var cookie = jQuery.trim(cookies[i]);
                // Does this cookie string begin with the name we want?
                if (cookie.substring(0, name.length + 1) == (name + '=')) {
                    cookieValue = decodeURIComponent(cookie.substring(name.length + 1));
                    break;
                }
            }
        }
        return cookieValue;
    }
};
/**

 * 增加悬浮窗口插件
 * Powered By Mr Zhou
 * QQ 627266138
 * E-mail 627266138qq.com
 * Date 2013-06-04
 * Dependence jquery-1.7.2.min.js
 **/



(function ($) {
  //调用方法 $('.xxx').showLevel({levelTitle:"1111",levelContent:333333333333""});
  $.fn.LevelDefaults = { //默认参数
      levelTitle: '123123',//浮动标题
      levelContent:'123123', //内容
      top: 0,//上浮动距离
      left:0,//左浮动距离
      margin:'0 0;', //边距
      warpWidth: '200', //浮动层宽度
      isBorder: true, //浮动层边框 true 为存在，
      borderColor:'ccc',// 边框颜色
      color: '000' //字体颜色
  };
  $.extendLevel = function (level,opt) { //obj 元素对象，opt 参数对象
    var g = {  //公共方法， 外部可调用
      //初始化
      init: function () {
        var html; //初始化浮动层
        html =  $('<br /><div class="level" style="display:none;">'
                    +'<a class="level_title">' + opt.levelTitle + '</a>'
                    + '<div class="level_content">' + opt.levelContent + '</div>'
                + '</div>');
        $(g.level).append(html); //将元素加入到g.level对象中
        var border = opt.isBorder ? '1px' : '0'; //判断边框是否存在
        if (opt.top != 0) $('.level', g.level).css({ 'top': opt.top }); //判断是否上浮
        if (opt.left != 0) $('.level', g.level).css({ 'left': opt.left });   //判断是否左浮

        $('.level',g.level).css({'margin': opt.margin, 'text-align': 'center', 'width': opt.warpWidth, 'border': '' + border + ' solid  #' + opt.borderColor + '', color: '#' + opt.color + '', 'position': 'absolute','display':'block' });
        $('.level_title', g.level).css({ 'padding': '4px 6px', 'text-align':'center','width':opt.warpWidth });
        $('.level_content', g.level).css({ 'margin': '2px ' + opt.warpWidth - 4 + 'px', 'width': opt.warpWidth - 4, 'display':'block'});
        g.level.hover(//鼠标悬浮
            function () {
              g.level.children('.level').css({ 'display': 'block' });},
            function () {
              g.level.children('.level').css({ 'display': 'none' });
            });
      }
    };
    g.level = $(level);
    g.init();
    return g;
  }
  $.fn.showLevel = function (options) {
    if (this.length == 0) return; //判断对象是否存在
    this.each(function () {
      if (this.usedLevel) return;
      var opt = $.extend({}, $.fn.LevelDefaults, options); //合并已赋值参数
      this.usedLevel = $.extendLevel(this, opt);
    });
  }
})(jQuery);