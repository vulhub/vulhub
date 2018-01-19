<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title><?php if($con['seo_title']==''){echo $con['app_title'].'免费下载|'.$con['app_title'].'手机版下载 - '.SITE_NAME;}
else{echo $con['seo_title'].'-'.SITE_NAME;}?></title>
<meta name="keywords" content="<?php if($con['seo_keywords']==''){echo $con['app_title'];}
else{echo $con['seo_keywords'];}?>" />
<meta name="description" content="<?php if($con['seo_desc']==''){$desc=$c->deletehtml($con['app_title'].','.$con['app_desc']);
if(strlen($desc)>358)
{echo substr($desc,0,356).'...';}
else
{echo $desc;}}
else{echo $con['seo_desc'];}?>" />
<script language="javascript" type="text/javascript" src="<?php echo SITE_PATH;?>templates/lib/jquery-1.7.1.min.js" ></script>
<link rel="stylesheet"  href="<?php echo SITE_PATH;?>templates/<?php echo TEMPLATE;?>/css/style.css"  type="text/css" />
<script type="text/javascript" src="<?php echo SITE_PATH;?>templates/<?php echo TEMPLATE;?>/css/js/comm.js"></script>
<?php require_once ( 'inc_head.php');?>
    <p class="line-t-15"></p>
        <div class="warp-content"> <!-- 主体内容 开始 -->
        <div class="head-locate">
            <span>您当前位置：</span>
            <?php
            //exit(print_r($con));
            $nav = "";
            foreach($c->cate_father($con['last_cate_id']) as $n){
                $nav .= '<a href="'.$n['surl'].'">'.$n['cname'].'</a>&nbsp;>&nbsp;';
            }
            echo $nav;
            ?>
            <span><?php echo $con['app_title'];?></span>
        </div>
        <div class="marauto">

            <div class="l body-left"> <!-- 左侧主体内容 -->
                <div class="bor-sty bg-fff bout-content app-info">
                    <div class="code-right">
                        <?php // echo $c->google_qrcode(urlencode(SITE_URL.SITE_PATH.'download.php?hid='.$con['history'][0]['appcms_history_id']),"140");
				echo $c->liantu_qcode(urlencode(SITE_URL.SITE_PATH.'download.php?hid='.$con['history'][0]['appcms_history_id']), 140, $logo = null);
?>
                        <p>扫描二维码，快速下载本应用</p>
                        <p>推荐用 <a href="<?php echo(SITE_PATH.'index.php?tpl=search&q='.urlencode('我查查'));?>" style="color:blue">我查查</a> 扫描</p>
                    </div>
                    <div class="app-info-i">
                        <a href="<?php echo $con['surl']?>" class="l rank-img"><img src="<?php echo $con['app_logo']?>" border="0" alt=""></a>
                        <div class="app-info-n">
                            <a class="app-title" href="<?php echo $con['surl'];?>"><b><?php echo $con['app_title'];?></b></a>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="col-94 download-num">下载： <?php echo $con['app_down'];?></span>&nbsp;&nbsp;<span class="col-94 download-num">点击： <?php echo $con['app_visitors'];?></span>
                            <ul class="level"><li class="l<?php echo ceil($con['app_recomment']/2);?>"></li></ul>
                        </div>
                        <br /><br />
                        <div class="detailed-info">
                            <span>大小：<?php echo $con['app_size'];?></span>
                            <span>更新日期：<?php echo date("Y-m-d",$con['app_update_time']);?></span>
                            <span>类型：<?php echo $con['app_type'];?></span>
                            <span>最新版本：<?php echo $con['app_version'];?></span>
                            <span>开发者：<?php echo $con['app_company'];?></span>
                        </div>
                    </div>
                    <div class="down-install">
                        <a href="javascript:void(0);" onclick="window.open('<?php echo(SITE_PATH.'download.php?id='.$con['history'][0]['appcms_history_id']);?>');" class="down-computer"></a>&nbsp;&nbsp;
                        <script src="http://wandoujia.com/api/wdapi.js" type="text/javascript"></script>
                        <script type="text/javascript">
                        function down_apk(a){
                            $.getJSON('<?php echo(DATA_CENTER_URL) ?>/apkjson.php?jsoncallback=?',{"hid":<?php echo($con['history'][0]['appcms_history_id']);?>},function(json){
                                try{$('#tdownurl').attr({"href":json.url});return wdapi_apkdl_m(a, 'api_143');}catch(e){alert(e.message);}
                            });
                            return false;
                        }
                        </script>
                        <a class="install" name="<?php echo $con['app_title'];?>" id="tdownurl" href="javascript:void(0);" onclick="return down_apk(this);" title="安装豌豆荚，把手机连接到电脑后，可以直接安装到手机"></a>
                        <br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;如果无法下载，<span>点此 <a style="color:blue;" href="javascript:void(0)" onClick="setbug('<?php echo $con['history'][0]['appcms_history_id']?>','<?php echo DATA_CENTER_URL;?>')">提交错误</a></span>
                    </div>
                    <p class="hr"></p>
                    <div class="screenshot">
                        <div class="ibox">
                            <ul class="date_history">
                                <li class="date_his_list date_bluebg">
                                    <ul class="date_his_son">
                                        <li>版本号</li>
                                        <li>大小</li>
                                        <li>更新时间</li>
                                        <li>版本要求</li>
                                        <li style="text-align:right;">立即下载</li>
                                    </ul>
                                </li>
                                <?php foreach($con['history'] as $his){// print_r($con['history']); exit;?>
                                <li class="date_his_list">
                                    <ul class="date_his_son">
                                        <li><a href="<?php echo $his['surl']; ?>"><?php echo $his['app_version']; ?></a></li>
                                        <li><?php echo $his['app_size']; ?></li>
                                        <li><?php echo date("Y-m-d",$his['app_update_time']);?></li>
                                        <li><?php echo $his['app_system']; ?></li>
                                        <li><a class="his_down" style="float:right;" href="<?php echo $his['surl']; ?>" title="下载"></a></li>
                                    </ul>
                                </li>
                                <?php } ?>
                            </ul>
                        </div>


                    </div>
                    <div class="introduce">
                        <h3><b><?php echo $con['app_title']; ?> 应用介绍</b></h3>
                        <br />
                        <p class="in-info">
                            <?php echo $con['app_desc']; ?>
                        </p>
                        <p class="line-t-25"></p>
                        <h3><b><?php echo $con['app_title']; ?> 应用截图</b></h3>
                        <br />
                        <div class="scrbg">
                        <div class="screen-img">
                            <center>
                                <p class="line-t-25"></p>
                                <?php if(!empty($con['resource'])){foreach( $con['resource'] as $k => $v){?>
                                    <img src="<?php echo $v['resource_url']; ?>" onload="this.width > 600 ? this.width=600 : this.width" border="0" alt="<?php echo $con['app_title']; ?>" style="margin-bottom:10px;">
                                <?php } }?>
                                <p class="line-t-10"></p>
                            </center>
                        </div>
                        </div>
                    </div>
                    <div class="comment">
                        <?php echo $con['comment_code'];?>

                    </div>

                </div>
            </div><!-- 左侧主体内容  结束 -->
            <!--右侧内容-->
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
