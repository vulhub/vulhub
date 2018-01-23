<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>专题 - <?php echo SITE_NAME;?></title>
<script language="javascript" type="text/javascript" src="<?php echo SITE_PATH;?>templates/lib/jquery-1.7.1.min.js" ></script>
<link rel="stylesheet"  href="<?php echo SITE_PATH;?>templates/<?php echo TEMPLATE;?>/css/style.css"  type="text/css" />
<script type="text/javascript" src="<?php echo SITE_PATH;?>templates/<?php echo TEMPLATE;?>/css/js/comm.js"></script>
<?php require_once ('inc_head.php');?>
<p class="line-t-15"></p>
    <div class="warp-content"> <!-- 主体内容 开始 -->
        <div class="l body-left"> <!-- 左侧主体内容 -->
        <?php $a = $c->get_recommend(array('pagesize'=>PAGESIZE,'count'=>1,'rewrite'=>SITE_REWRITEURL,'where'=>'(area_type=4 or area_type=3)','p'=>$p));//print_r($a);?>
            <div class="bor-sty bg-fff pdb30 topics-info">
                <span class="topics-title">专题列表</span>
                <div class="topics-list">
                <?php if(!empty($a['list'])){foreach($a['list'] as $k => $v ){
                        if(empty($v['area_logo'])){$v['area_logo']=SITE_PATH."templates/".TEMPLATE."/css/img/default.jpg";}
                ?>
                    <div class="topics">
                        <a class="l topic-item-img" href="<?php echo $v['surl'];?>"><img onerror="this.src='<?php echo SITE_PATH."templates/".TEMPLATE."/css/img/default.jpg";?>';" src="<?php echo $v['area_logo'];?>" border="0" alt="<?php echo $v['title'];?>"></a>
                        <div class="r tpc-info">
                            <h3><a href="<?php echo $v['surl'];?>"><b><?php echo $v['title'];?></b></a></h3>
                            <p><?php echo $v['area_html'];?></p>
                            <p class="line-t-10"></p>
                            <p class="apps">
                                <?php 
                                    if($v['area_type'] == 3){ 
                                        $lists = $c->get_recommend(array('area_id'=>$v['area_id'],'pagesize'=>5,'count'=>0,'rewrite'=>SITE_REWRITEURL));
                                        echo('<span class="colCCC">本专题有 '.count($lists['list']).' 个应用，点击图标查看详情</span><br>');
                                        foreach($lists['list'] as $k => $val){
                                ?>
                                            <a href="<?php echo $val['surl']?>" title="<?php echo $val['app_title']?>"><img src="<?php echo $val['app_logo']?>" border="0" alt="<?php echo $val['app_title']?>"></a>
                                <?php 
                                        } 
                                    } 
                                ?>
                            </p>
                        </div>
                    </div>
                    <p class="hr2"></p>
                    <p class="line-t-28"></p>
                    <?php } } ?>
                </div>
                <div class="pagebar">
                            <?php echo $a['pagebar']['pagecode']?>
                </div>
                <P class="line-t-15"></p> 
            </div>
        </div><!-- 左侧主体内容  结束 -->
        <?php require_once ('inc_right.php');?>
    </div> <!-- 主体内容 结束 -->
    <p class="line-t-15"></p>

    <?php require_once ('inc_foot.php');?>