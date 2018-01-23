<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title><?php echo $c->categories[$cid]['cname'].' - '.SITE_NAME;?></title>
<script language="javascript" type="text/javascript" src="<?php echo SITE_PATH;?>templates/lib/jquery-1.7.1.min.js" ></script>
<link rel="stylesheet"  href="<?php echo SITE_PATH;?>templates/<?php echo TEMPLATE;?>/css/style.css"  type="text/css" />
<script type="text/javascript" src="<?php echo SITE_PATH;?>templates/<?php echo TEMPLATE;?>/css/js/comm.js"></script>
<?php require_once ('inc_head.php');?>
    <div class="warp-content"> <!-- 主体内容 开始 -->
        <div class="marauto">
        <div class="locate">
                 <span>类别：
                 <?php 
                    if(intval($c->categories[$cid]['parent_id'])>0){
                        echo('<a href="'.$c->categories[$c->categories[$cid]['parent_id']]['surl'].'" '.$style.'>全部</a>');
                    }else{
                        echo('<a href="'.$c->categories[$cid]['surl'].'" '.$style.'>全部</a>');
                    }
                    if($c->categories[$cid]['parent_id']==0){
                        $a=$c->cate_son($cid);
                    }else{
                        $a=$c->cate_son($c->categories[$cid]['parent_id']);
                    }
                    foreach($a as $b){
                        $style='';
                        if($b['cate_id'] == $cid) $style='class="loc-selected"';
                        echo('<a href="'.$b['surl'].'" '.$style.'>'.$b['cname'].'</a>');
                    }
                 ?>
                 </span>
            </div>
            <p class="line-t-10"></p>
			<?php $infos = $c->get_list(array('type'=>'1','count'=>'1','cate_id'=>$cid,'rewrite'=>SITE_REWRITEURL,'pagesize'=>PAGESIZE,'p'=>$p));//print_r($infos);?>
            <div class="l body-left"> <!-- 左侧主体内容 -->
                <div class="bor-sty bg-fff consult-list">
					<?php if(!empty($infos['list'])){foreach($infos['list'] as $k => $i){
                        if(empty($i['info_img'])){$i['info_img']=SITE_PATH."templates/".TEMPLATE."/css/img/default.jpg";}
					?>
                    <div class="cst-list">
                        <span class="cst-head">
                            <span class="r disInBlock martop">浏览：<?php echo $i['info_visitors'];?></span>
                            <h2><b><a href="<?php echo $i['surl'];?>" class="cst-title"><?php echo $i['info_title'];?></a></b></h2>
                        </span>
                          <p class="cst-info">
                            <?php echo $i['info_desc'];?>
                        </p>
                        <a class="cst-img" style="_float:left;" href="<?php echo $i['surl'];?>"><img src="<?php echo $i['info_img'];?>" border="0" alt="<?php echo $i['info_title'];?>"></a>
                        <a href="<?php echo $i['surl'];?>" class="go-cst" target="_blank">阅读全文></a>
                    </div>
                    <?php } }else{ echo '<div class="h788"><span>没有找到数据</span></div>';}?>
                     <div class="pagebar">
                            <?php if(!empty($infos['list'])) echo $infos['pagebar']['pagecode']?>
                    </div>
                    <P class="line-t-15"></p> 
                </div>
            </div><!-- 左侧主体内容  结束 -->
			<?php require_once ('inc_right.php');?>
        </div>
    </div> <!-- 主体内容 结束 -->
    <p class="line-t-15"></p>


<?php require_once ('inc_foot.php');?>