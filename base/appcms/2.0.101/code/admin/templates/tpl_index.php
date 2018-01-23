<?php require_once (dirname(__FILE__) . '/inc_header.php');?>
<link rel="stylesheet" media="screen" type="text/css" href="templates/css/login.css" />
<script type="text/javascript">
    $(document).ready(function () {
        adjust();//调整
    });
    $(window).resize(function () {
        adjust();//调整
    });
    function adjust() {
        $('.warp').css({'margin-top':(($(window).height() - $('.warp').height()) / 2-40)+'px'});
        $('.hsize').css({ 'margin-left': ($(window).width() - $('.warpLogin').width()) / 2 - 40 });//计算文字距离
        $('.warpLogin').css({'margin-left':($(window).width() - $('.warpLogin').width()) / 2});
    }
</script>
</head>
<body>
<div class="warp">
<input id="sitename" type="hidden" value='<?php echo SITE_NAME; ?>' />
<input id="authcode" type="hidden" value='<?php echo AUTH_CODE; ?>' />
<input id="sdomain" type="hidden" value='<?php echo str_replace("http://","",SITE_URL); ?>' />
    <h2 class="hsize">欢迎登录&nbsp;&nbsp; AppCMS</h2>
    <div class="warpLogin">
        <div class="warpContent">
            <ul>
            <li class="th">帐　号：</li>
            <li class="td">
                <input type="text" tabindex="1" id="uname" value="" class="userName" onKeyDown="if(event.keyCode==13) login();" /></li>
            <li class="th">密　码：</li>
            <li class="td">
                <input type="password" tabindex="2" id="upass" value="" class="userPWD" onKeyDown="if(event.keyCode==13) login();" /></li>
            <li class="th">安全码：</li>
            <li class="td">
                <input type="password" id="safecode" tabindex="3" value="" class="safecode" onKeyDown="if(event.keyCode==13) login();" /></li>
            <li class="th">验证码：</li>
            <li class="td">
                <input tabindex="4" type="text" id="code" value="" class="verificationCode" onKeyDown="if(event.keyCode==13) login();" />&nbsp;&nbsp;<img src="<?php echo SITE_PATH?>getcode.php?c=login" style="border: 0;" class="checkCode" id="checkCode" onClick="document.getElementById('checkCode').src='<?php echo SITE_PATH?>getcode.php?c=login&random='+Math.random();" /></li>

            </ul>
        </div>
        <p class="warpBtn"><a id="admin_login" tabindex="5" class="btnLogin" href="javascript:void(0);" onClick="login();">登&nbsp;录</a></p>
    </div>
</div>
</body>
</html>