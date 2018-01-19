<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title><?php echo '安卓应用下载 - '.SITE_NAME;?></title>
<script language="javascript" type="text/javascript" src="<?php echo SITE_PATH;?>templates/lib/jquery-1.7.1.min.js" ></script>
<link rel="stylesheet"  href="<?php echo SITE_PATH;?>templates/<?php echo TEMPLATE;?>/css/style.css"  type="text/css" />
<script type="text/javascript" src="<?php echo SITE_PATH;?>templates/<?php echo TEMPLATE;?>/css/js/comm.js"></script>
<?php require_once ("inc_head.php")  ;?>
    <div class="warp-content"> <!-- 主体内容 开始 -->
        <div class="marauto">
            <div class="locate">
                 <span>
                 <?php 
                    $a=$c->cate_son(1);
                    foreach($a as $b){
                        $style='';
                        if($b['cate_id'] == $cid) $style='class="loc-selected"';
                        echo('<a href="'.$b['surl'].'" '.$style.'>'.$b['cname'].'</a>');
                    }
                    $a=$c->cate_son(2);
                    foreach($a as $b){
                        $style='';
                        if($b['cate_id'] == $cid) $style='class="loc-selected"';
                        echo('<a href="'.$b['surl'].'" '.$style.'>'.$b['cname'].'</a>');
                    }
                 ?>
                 </span>
            </div>
            <p class="line-t-10"></p>
            <div > <!-- 左侧主体内容 -->
                <div class="pr zIndex10 tab-content">
                    <div class="bor-sty bg-fff">
                    <?php $lists = $c->get_list(array('pagesize'=>40,'count'=>1,'cate_id'=>$cid,'rewrite'=>SITE_REWRITEURL,'p'=>$p));
                    //print_r($lists['sql']);
                    ?>
                            <table class="tb">
                                <tr>
                                    <th>名称</th>
                                    <th>版本号</th>
                                    <th>应用大小</th>
                                    <th>下载量</th>
                                    <th>访问量</th>
                                    <th>评分</th>
                                    <th>更新时间</th>
                                </tr>
                        <?php foreach($lists['list'] as $key=> $val){?>
                                <tr>
                                    <td style="text-indent:10px;text-align:left;"><a href="<?php echo $val['surl']; ?>"><?php echo $val['app_title']; ?></a></td>
                                    <td><?php echo $val['app_version']; ?></td>
                                    <td><?php echo $val['app_size']; ?></td>
                                    <td><?php echo $val['app_down']; ?></td>
                                    <td><?php echo $val['app_visitors']; ?></td>
                                    <td><?php echo $val['app_recomment']; ?></td>
                                    <td><?php echo date("Y-m-d",$val['app_update_time']);?></td>
                                </tr>
                        <?php }?>    
                            </table>
                        <div class="pagebar">
                            <?php if(!empty($lists['list']))echo $lists['pagebar']['pagecode']?>
                        </div>
                        <P class="line-t-15"></p> 
                    </div>
                </div>
                
            </div><!-- 左侧主体内容  结束 -->
            <?php //require_once (dirname(__FILE__) . '/inc_right.php');?>
            <!-- 右侧主体内容  结束 -->
        </div>
    </div> <!-- 主体内容 结束 -->
    <p class="line-t-15"></p>
    
    <?php require_once ('inc_foot.php');?>