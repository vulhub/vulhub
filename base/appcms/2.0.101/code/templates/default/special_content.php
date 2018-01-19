<?php 
    $lists = $c->get_recommend(array('area_id'=>$id,'pagesize'=>PAGESIZE,'count'=>0,'rewrite'=>SITE_REWRITEURL,'resource'=>1,'where'=>'(area_type=4 or area_type=3)')); //print_r($lists);
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title><?php echo $lists['title'].' - '.SITE_NAME;?></title>
<script language="javascript" type="text/javascript" src="<?php echo SITE_PATH;?>templates/lib/jquery-1.7.1.min.js" ></script>
<link rel="stylesheet"  href="<?php echo SITE_PATH;?>templates/<?php echo TEMPLATE;?>/css/style.css"  type="text/css" />
<script type="text/javascript" src="<?php echo SITE_PATH;?>templates/<?php echo TEMPLATE;?>/css/js/comm.js"></script>
<?php require_once ('inc_head.php');?>
    <p class="line-t-15"></p>
    <div class="warp-content"> <!-- 主体内容 开始 -->
        <div class="l body-left"> <!-- 左侧主体内容 -->
        <?php 
            if(empty($lists['area_logo'])){$lists['area_logo']=SITE_PATH."templates/".TEMPLATE."/css/img/default.jpg";}
        ?>
            <div class="bor-sty pdb30 bgGrey topics-info">
                <div class="tpc-head bg-fff">
                    <div class="topics pad-t-b" style="height:auto;">
                        <a class="l topic-item-img" style="margin:0 10px 10px 0;" ><img src="<?php echo $lists['area_logo']?>" border="0" alt=""></a>
                        <h3><b><?php echo $lists['title']?></b></h3>
                        <?php echo $lists['area_html']?> 
                        <div style="clear:both;"></div>
                    </div>
                </div>
                
                <?php if($lists['area_type']==3){?>
                <div class="introduce">
                    <?php foreach($lists['list'] as $k => $v){ ?>
                    <div class="item-list">
                        <a href="<?php echo $v['surl'];?>" class="l item-ico"><img src="<?php echo $v['app_logo'];?>" border="0" alt="<?php echo $v['app_title'];?>"></a>
                        <em class="arrow"></em>
                        <div class="l item-info bg-fff">
                            <a href="<?php echo $v['surl'];?>" class="r down-computer"></a>
                            <h3><b><a href="<?php echo $v['surl'];?>" class="item-title"><?php echo $v['app_title'];?></a></b></h3>
                            <ul class="level"><li class="l<?php echo ceil($v['app_recomment']/2)?>"></li></ul>
                            <br />
                            <span>版本：<?php echo $v['app_version'];?> &nbsp;&nbsp;|&nbsp;&nbsp;更新日期：<?php echo date("Y-m-d",$v['app_update_time']);?></span>
                            <p class="line-t-15"></p>
                            <span class="font-grey">
                                <p><?php echo $v['app_desc'];?> </p>
                                
                            <p class="line-t-15"></p>
                            <div class="look-img">
                            <?php if(!empty($v['resource'])){ foreach($v['resource'] as $rk => $rv){
                                if($rk==3){break;}
                            ?>
                                <img src="<?php echo $rv['resource_url']?>" width="480" height="800" border="0" alt="<?php echo $v['app_title'];?>">
                            <?php } }?>
                            </div>
                            <a href="<?php echo $v['surl'];?>" class="go-cst">阅读全文></a>
                        </div>
                    </div>
                    <?php } }?>
                </div>
                
            </div>
        </div><!-- 左侧主体内容  结束 -->
        <!-- 右侧主体内容 -->
        <?php require_once ('inc_right.php');?>
            <!-- 右侧主体内容  结束 -->
    </div> <!-- 主体内容 结束 -->
    <p class="line-t-15"></p>
     <?php require_once ('inc_foot.php');?>