<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title><?php echo $con['info_title'].' - '.SITE_NAME;?></title>
<script language="javascript" type="text/javascript" src="<?php echo SITE_PATH;?>templates/lib/jquery-1.7.1.min.js" ></script>
<link rel="stylesheet"  href="<?php echo SITE_PATH;?>templates/<?php echo TEMPLATE;?>/css/style.css"  type="text/css" />
<script type="text/javascript" src="<?php echo SITE_PATH;?>templates/<?php echo TEMPLATE;?>/css/js/comm.js"></script>
<?php require_once ('inc_head.php');?>
<p class="line-t-15"></p>
    <div class="warp-content"> <!-- 主体内容 开始 -->
        <div class="marauto">
            <div class="l body-left"> <!-- 左侧主体内容 -->
                 <div class="bor-sty bg-fff consult-info pdb30">
                    <div class="detail-info">
                        <p class="line-t-10"></p>
                        <h2><b><?php echo $con['info_title'];?></b></h2>
                        <p class="line-t-10"></p>
                        <p><span>来源： <?php echo $con['info_from']==''?SITE_NAME:$con['info_from'];?></span>&nbsp;&nbsp;&nbsp;&nbsp;<span>时间：<?php echo date('Y-m-d H:i:s',$con['create_time']);?></span></p>
                        <p class="line-t-5"></p>
                        
                        <p class="hr"></p>
                        <p class="line-t-15"></p>
                        <p class="font textIndent20">
                            <?php 
                            echo $con['info_body'];?>
                        </p>
                        <div class="comment">
                        <?php echo $con['comment_code'];?>
                    </div>
                    </div>

                </div>
            </div><!-- 左侧主体内容  结束 -->
            <?php require_once (dirname(__FILE__) . '/inc_right.php');?>
        </div>
    </div> <!-- 主体内容 结束 -->
    
    <p class="line-t-15"></p>

    <script type="text/javascript" id="bdshare_js" data="type=tools&amp;uid=0" ></script>
<script type="text/javascript" id="bdshell_js"></script>
<script type="text/javascript">
document.getElementById("bdshell_js").src = "http://bdimg.share.baidu.com/static/js/shell_v2.js?cdnversion=" + Math.ceil(new Date()/3600000);
</script>
<?php require_once ('inc_foot.php');?>