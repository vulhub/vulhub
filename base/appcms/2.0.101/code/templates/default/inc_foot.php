<div class="warp-footer"> <!-- 页脚 结束 -->
      <div class="marauto">
        <div class="l footer-left">
            <h4>友情链接</h4>
            <div class="footer-link">
                <?php echo($c->get_flink($tpl));?>
            </div>
            <div class="f-img-link">
            
            </div>
        </div>
        <p class="line-t-25"></p>
        <span class="footer-info">
            <p class="line-t-15"></p>
            <!-- 版权标识开始 -->
            <!-- 如果取消了版权标识，AppCMS有权取消您的使用授权 -->
            <div class="l">程序支持: <a href="http://www.appcms.cc/">AppCMS</a> &copy; 2013&nbsp;&nbsp;&nbsp;&nbsp;<?php echo "<a href='".DATA_CENTER_URL."/appsubmit.php' target='_blank' >提交应用</a>";?></div>
            <!-- 版权标识结束 -->
            <div class="r"><?php echo('Processed in '.number_format(helper::getmicrotime()-$time_start, 2, '.', '').' MilliSeconds ');?>&nbsp;&nbsp;<?php if(defined('COUNT_CODE')) echo(COUNT_CODE); ?></div>
            <p class="line-t-25"></p>
        </span>
      </div>    
    </div><!-- 页脚 结束  -->
</div>
<div class="to-top"  style="display:none;"  >
	<a class="to-top-a" title="返回顶部"></a>
</div>

<script type="text/javascript">
	$(function(){
		$(window).scroll(function(){
				var scrolltop=$(this).scrollTop();
				scrolltop > 250 ? $('.to-top').show(): $('.to-top').hide();
				var top = $(window).height();
				$('.to-top').css({'top':scrolltop + top - 246});
			});
		$('.to-top').on('click',function(){
			if(!$('body').is(":animated"))
				$('html,body').animate({scrollTop: 0}, 500);
		});
	});
</script>
</body>
</html>
