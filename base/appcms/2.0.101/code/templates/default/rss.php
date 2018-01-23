<?php echo '<?xml version="1.0" encoding="utf-8"?>';?>
<rss version="2.0">
<channel>
    <title><?php echo SITE_NAME;?></title>
    <description><?php if(defined('SEO_DESCRIPTION')) echo SEO_DESCRIPTION;?></description>
    <link><?php echo SITE_URL;?></link>
    <?php  $a = $c->get_list(array('pagesize'=>50,'count'=>0,'cate_id'=>0,'resource'=>1,'rewrite'=>SITE_REWRITEURL,'order'=>'order by app_update_time desc')); //echo date("D M j H:i:s T+0800 Z",'1331215010878');?>
    <?php foreach($a['list'] as $k => $v){?>
    <item>
        <title><![CDATA[<?php echo $v['app_title']?>]]></title>
        <link><![CDATA[<?php echo $v['surl']?>]]></link>
        <pubDate><![CDATA[<?php echo date('D M j H:i:s T+0800 Z',$v['app_update_time']);?>]]></pubDate> 
        <description><![CDATA[
        <?php 
            $head =''; if(substr($v['app_logo'],0,1)=='/') $head = SITE_URL; 
            echo '<img src="'.$head.$v['app_logo'].'" align="left" />'; 
            echo helper::utf8_substr(strip_tags($v['app_desc']),0,500);
            echo '<div><a href="'.$v['surl'].'">'.$v['app_title'].'</a></div>';
        ?>
        ]]></description>
    </item>
    <?php }?>

    <?php $a = $c->get_list(array('pagesize'=>50,'count'=>0,'type'=>1,'resource'=>1,'cate_id'=>0,'rewrite'=>SITE_REWRITEURL,'order'=>'order by info_update_time desc')); //echo date("D M j H:i:s T+0800 Z",'1331215010878');?>
    <?php foreach($a['list'] as $k => $v){?>
    <item>
        <title><![CDATA[<?php echo $v['info_title']?>]]></title>
        <link><![CDATA[<?php echo $v['surl']?>]]></link>
        <pubDate><![CDATA[<?php echo date('D M j H:i:s T+0800 Z',$v['info_update_time']);?>]]></pubDate> 
        <description><![CDATA[
        <?php 
            $head =''; 
            if(substr($v['info_img'],0,1)=='/') $head = SITE_URL; 
            echo '<img src="'.$head.$v['info_img'].'" align="left" /> '; 
            echo helper::utf8_substr(strip_tags($v['info_desc']),0,500);
        ?>
        ]]></description> 
    </item>
    <?php }?>
</channel>
</rss>