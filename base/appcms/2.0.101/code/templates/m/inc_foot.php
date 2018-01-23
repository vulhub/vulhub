<div class="box wrap-footer">
<?php echo($c->get_flink($tpl));?>
	<span style="font-size:9px;">Powered by &nbsp;&nbsp;&nbsp;<a href="http://www.appcms.cc/">AppCMS</a>&nbsp;&nbsp;<?php echo('Processed in '.number_format(helper::getmicrotime()-$time_start, 2, '.', '').' ms ');?>&nbsp;&nbsp;<?php if(defined('COUNT_CODE')) echo(COUNT_CODE); ?></span>
</div>
