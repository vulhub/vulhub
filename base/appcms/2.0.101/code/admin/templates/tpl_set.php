<?php require_once(dirname(__FILE__).'/inc_header.php');?>
<script language="javascript" type="text/javascript" src="templates/css/js/set.js" ></script>
<style>
    #title_colorpanel table tr{ line-height:inherit; border-bottom:none;}
    a.close-own {
       display: block;width: 16px;height: 16px;position: absolute;outline: none;right: 7px;top: 8px;text-indent: 200px;overflow: hidden;
    }
    .tb3 tr {border:1px solid #f1f1f1; border-width: 0 0 1px 0;}
    a.close-own:hover{background-position: left -46px}
</style>
<?php require_once (dirname(__FILE__).'/inc_top.php');?>
<!-- 加载左侧导航栏 -->
<?php require_once(dirname(__FILE__)."/inc_menu.php");?>
<!-- 右侧区域开始 -->
<div class="right_body"> 
    <!-- 当前位置开始 -->
    <div class="snav">您的位置：<a href="frame.php">管理首页</a> » 网站配置</div><!-- 当前位置结束 -->
    
    <!-- 右侧主体内容开始 -->
    <div class="mbody">
        <div class="main " id="mbody" style="margin-top:0">
            <div class="tab_table">
               <div class="btn_box" style="padding-left:0px;height:28px;width:100%; clear:both;border:1px solid #ccc; border-width:0 0 1px 0;margin-bottom:5px;">
                    <a href="javascript:void(0);" class="current" id="bn1" >基本设置</a>
                    <a href="javascript:void(0);" id="bn2">上传设置</a>
                    <a href="javascript:void(0);" id="bn3">首页优化</a>
                    <a href="javascript:void(0);" id="bn4">手机版</a>
                    <a href="javascript:void(0);" id="bn5">其他设置</a>
                </div>
            </div>
            <div id="e_box">
                <form id="set" >
                    <div id="tab1">
                        <table class="tb3" style="clear:both;width:100%;" >
                            <tr>
                                <td width="160">站点名称：</td>
                                <td class="alignleft"> <input class="ipt" id="site_name" style="width:300px" value="<?php echo SITE_NAME;?>"> </td>
                            </tr>
                            <tr>
                                <td>站点域名：</td>
                                <td class="alignleft"><input class="ipt" id="site_url" style="width:300px" value="<?php echo SITE_URL;?>"> 网站首页网址，以http://开头，最后不要带斜杠 / </td>
                            </tr>
							
                            <tr>
                                <td width="160">站点LOGO：</td>
                                <td class="alignleft">
                                    <div class="l">
                                    <input type="text" class="ipt" id="site_logo" name="site_logo" value="<?php if(defined('SITE_LOGO')) echo SITE_LOGO;?>"/></div>
                                    <div class="l" style="margin-left:10px;margin-left:5px \9;vertical-align:bottom;">
                                        <script>C.create_upload_iframe('{"func":"callback_upload","thumb":{"width":"300","height":"300"}}');</script>
                                    </div>
                                    LOGO图片高度不要超过70像素，宽度不要超过500像素
                                </td>
                            </tr>
                            
                            <tr>
                                <td>PC站模板风格：</td>
                                <td class="alignleft" ><?php show();?> <font color=red> * 更换模板时，为防止第三方制作模板含有安全隐患，请注意弹出的安全提示信息</font>&nbsp;&nbsp;<a href="http://bbs.appcms.cc">官方模版免费下载</a>&nbsp;&nbsp;<a href="http://bbs.appcms.cc">模版安装教程</a></td>
                            </tr>
                            <tr>
                                <td>评论代码：</td>
                                <td class="alignleft" >
                                    <div class="l">
                                        <textarea id="comment_code" rows="5" cols="60" style="font-size:12px;line-height:14px;"><?php echo urldecode(COMMENT_CODE);?></textarea>
                                    </div>
                                    <div class="l" style="width:300px;margin-left:10px;text-indent:0px;">
                                        在这里粘贴申请的社会化评论的JS代码，社会化评论有助于网站流量提升。如果留空，将会显示默认本地评论。社会化评论申请：<a href="http://www.duoshuo.com" target="_blank">多说</a>
                                    </div>
                                </td>
                            </tr>
                            <tr>
                                <td>统计代码：</td>
                                <td class="alignleft" >
                                    <div class="l">
                                        <textarea id="count_code" rows="5" cols="60" style="font-size:12px;line-height:14px;"><?php if(defined('COUNT_CODE')) echo COUNT_CODE;?></textarea>
                                    </div>
                                    <div class="l" style="width:300px;margin-left:10px;text-indent:0px;">
                                        在这里粘贴申请的统计代码，如 <a href="http://zhanzhang.cnzz.com" target="_blank">CNZZ统计</a>，<a href="http://tongji.baidu.com" target="_blank">百度统计</a>
                                    </div>
                                </td>
                            </tr>
                            <tr>
                                <td>&nbsp;</td>
                                <td><a href=" javascript:void(0);" class="but2" id="subtn"  onclick="save()" >确 定</a></td>
                            </tr>
                        </table>
                    </div>
                    <div id="tab2">
                        <table class="tb3" >
                            <tr>
                                <td style="width:160px;">资源上传路径：</td>
                                <td class="alignleft" ><input class="ipt" id="upload_path" value="<?php echo UPLOAD_PATH;?>"> * 资源上传路径,相对于上传文件夹 upload</td>
                            </tr>
                            <tr>
                                <td>上传安全码：</td>
                                <td class="alignleft" ><input class="ipt" id="upload_code" value="<?php echo UPLOAD_CODE;?>"> * 上传安全码，建议不修改，该值安装时候自动生成，6-12位随机字母（大小写）数字，特殊符号</td>
                            </tr>
                            <tr>
                                <td>上传密钥：</td>
                                <td class="alignleft" ><input class="ipt" id="upload_key" value="<?php echo UPLOAD_KEY;?>"> * 上传密钥，建议不修改，该值安装时候自动生成，6-12位随机字母（大小写）数字，特殊符号</td>
                            </tr>
                            <tr>
                                <td style="width:160px;">APK上传路径：</td>
                                <td class="alignleft" ><input class="ipt" id="upload_path_apk" value="<?php echo defined('UPLOAD_PATH_APK')?UPLOAD_PATH_APK:'/apk/';?>"> * 资源上传路径,相对于上传文件夹 upload，需要开启PHP的ZIP模块</td>
                            </tr>
                            <tr>
                                <td>&nbsp;</td>
                                <td><a href=" javascript:void(0);" class="but2" id="subtn"  onclick="save()" >确 定</a></td>
                            </tr>
                        </table>  
                    </div>
                    <div id="tab3">
                        <table class="tb3" style="clear:both;width:100%;" >
                            <tr>
                                <td width="160">标题（title）：</td>
                                <td class="alignleft"><input class="ipt" id="seo_title" name="seo_title" style="width:300px" value="<?php if(defined('SEO_TITLE')) echo SEO_TITLE ;?>"></td>
                            </tr>
                            <tr>
                                <td>关键字（keywords）：</td>
                                <td class="alignleft"><input class="ipt" id="seo_keywords" name="seo_keywords" style="width:300px" value="<?php if(defined('SEO_KEYWORDS')) echo SEO_KEYWORDS ;?>"></td>
                            </tr>
                            <tr>
                                <td>描述（description）：</td>
                                <td class="alignleft">
                                <textarea id="seo_description" rows="5" cols="60" style="font-size:12px;line-height:14px;"><?php if(defined('SEO_DESCRIPTION')) echo SEO_DESCRIPTION;?></textarea></td>
                            </tr>
                            
                            <tr>
                                <td>&nbsp;</td>
                                <td><a href=" javascript:void(0);" class="but2" id="subtn"  onclick="save()" >确 定</a></td>
                            </tr>
                        </table>
                    </div>
                    <div id="tab4">
                        <table class="tb3" style="clear:both;width:100%;" >
                            <tr>
                                <td width="160">是否开启手机版：</td>
                                <td class="alignleft" ><?php echo vars::input_str(array('node'=>'content_mobile','type'=>'radio','default'=> defined('CONTENT_MOBILE') ? CONTENT_MOBILE : 0))?> 开启该项可以让用户使用手机浏览器访问网站时自适应手机屏幕</td>
                            </tr>  
                            <tr>
                                <td>手机版独立域名：</td>
                                <td class="alignleft"><input class="ipt" id="wap_url" style="width:300px" value="<?php echo defined('WAP_URL') ? WAP_URL : '';?>"> 访问此域名时一直显示手机版，以http://开头，最后不要带斜杠 / </td>
                            </tr>
							<tr>
                                <td >手机版LOGO：</td>
                                <td class="alignleft">
                                    <div class="l">
                                    <input type="text" class="ipt" id="wap_logo" name="wap_logo" value="<?php if(defined('WAP_LOGO')) echo WAP_LOGO;?>"/></div>
                                    <div class="l" style="margin-left:10px;margin-left:5px \9;vertical-align:bottom;">
                                        <script>C.create_upload_iframe('{"func":"callback_upload_waplogo","thumb":{"width":"300","height":"300"}}');</script>
                                    </div>
                                    LOGO图片高度不要超过50像素，宽度不要超过300像素
                                </td>
                            </tr>
							<tr>
                                <td>手机版模板风格：</td>
                                <td class="alignleft" ><?php show_wap();?> <font color=red> * 更换模板时，为防止第三方制作模板含有安全隐患，请注意弹出的安全提示信息</font></td>
                            </tr>
                            <tr>
                                <td>&nbsp;</td>
                                <td><a href=" javascript:void(0);" class="but2" id="subtn"  onclick="save()" >确 定</a></td>
                            </tr>
                        </table>
                    </div>
                    <div id="tab5">
                        <table class="tb3" style="clear:both;width:100%;" >
                            <tr>
                                <td width="160">是否开启伪静态重写：</td>
                                <td class="alignleft" ><?php echo vars::input_str(array('node'=>'site_rewriteurl','type'=>'radio','default'=>SITE_REWRITEURL))?> Apache 环境会自动修改 .htaccess 文件，<font color=red>IIS 则需要配合 IIS_Rerite 组件并自己修改相应的伪静态配置使用</font></td>
                            </tr> 
							 <tr>
                                <td width="160">是否开启图片地址伪静态：</td>
                                <td class="alignleft" ><?php echo vars::input_str(array('node'=>'img_rewrite','type'=>'radio','default'=> defined('IMG_REWRITE') ? IMG_REWRITE : 0))?> 开启该项可以增强SEO最佳效果，应用数过多会降低性能</td>
                            </tr> 
                            <tr>
                                <td>正文内部链接：</td>
                                <td class="alignleft" ><?php echo vars::input_str(array('node'=>'content_nlink','type'=>'radio','default'=>CONTENT_NLINK))?> 开启该项可以通过内部关键词链接增强SEO效果，关键词数量过大会牺牲一定性能</td>
                            </tr>
                            <tr>
                                <td>默认每页显示行数：</td>
                                <td class="alignleft" ><input class="ipt" id="pagesize" style="width:50px" value="<?php echo PAGESIZE;?>"> 列表默认显示条数</td>
                            </tr>
							<tr>
                                <td>缓存时间：</td>
                                <td class="alignleft" ><input class="ipt" id="cache_time" style="width:50px" value="<?php if(defined('CACHE_TIME')) echo CACHE_TIME;?>"> 缓存时间，0或不填为不缓存，以秒计算</td>
                            </tr>
							<tr>
                                <td>赚钱联盟ID：</td>
                                <td class="alignleft" ><input class="ipt" id="union_id" style="width:50px" value="<?php if(defined('UNION_ID')) echo UNION_ID;?>"> 赚钱联盟的账号数字ID，在 <a href="http://union.appcms.cc" target="_blank">http://union.appcms.cc</a> 中注册登录后即可获取</td>
                            </tr>
                            <tr>
                                <td>后台登录认证码：</td>
                                <td class="alignleft" ><input class="ipt" id="site_safecode" style="width:300px" value="<?php echo SITE_SAFECODE;?>"> 该认证码用于提升后台安全性，可留空</td>
                            </tr> 
                            <tr>
                                <td>授权码：</td>
                                <td class="alignleft" ><input class="ipt" id="auth_code" style="width:300px" value="<?php echo AUTH_CODE;?>"> 
                                此授权码绑定当前唯一域名，在添加应用处申请，请勿遗失和暴露给他人<br>
                                <font color=red>请勿随便修改该值，否则将导致无法添加应用</font>，找回已申请的授权码联系客服提供域名和申请时填写的QQ号码
                                </td>
                            </tr> 
							<tr>
                                <td>百度提交完整链接：</td>
                                <td class="alignleft" ><input class="ipt" id="baidu_submit" style="width:300px" value="<?php echo BAIDU_SUBMIT;?>"> 
                                <font color=red>请到<a href="http://zhanzhang.baidu.com/">百度站长平台</a>申请KEY,获取此链接.</font>
                                </td>
                            </tr>
                            <tr>
                                <td>&nbsp;</td>
                                <td><a href=" javascript:void(0);" class="but2" id="subtn"  onclick="save()" >确 定</a></td>
                            </tr>
                        </table>
                    </div>
                </form> 
            </div>
        </div>
    </div><!-- 右侧主体内容结束 -->
    
    <?php require_once(dirname(__FILE__)."/inc_footer.php");?>

</div><!-- 右侧区域结束 -->

<!-- 隐藏表单半透明层 -->
</body>
</html>