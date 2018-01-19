<!-- 底部版权开始 -->
<div class="mbody mfoot" style="clear:both;"> 
    <div class="l">
        &copy; 2013 <a href="http://www.appcms.cc" target="_blank">AppCMS</a> All rights Reserved.
    </div>
    <div class="r">
        <?php echo('Processed in '.number_format(helper::getmicrotime()-$time_start, 2, '.', '').' MilliSecond '.$dbm->query_count.' Queries ');?>&nbsp;&nbsp;
    </div>
</div> <!-- 底部版权结束 -->
<!--计算页面高度-->