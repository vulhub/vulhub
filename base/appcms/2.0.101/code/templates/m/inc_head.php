<div class="box wrap-head">
	<div class="box logo">
		<a href="<?php  echo 'http://'.$_SERVER['HTTP_HOST'];?>">
			<?php if(defined('WAP_LOGO') && WAP_LOGO != ''){ echo '<img border="0" src="'.WAP_LOGO.'">';}?>
		</a>
	</div>
	<div class="box search-box">
		<script type="text/javascript">
			/*搜索函数*/
			function do_search(){
				var searchvalue = document.getElementById('abc').value;
				if(searchvalue==''){return alert('请输入关键词');}
				window.location.href="<?php echo SITE_PATH?>index.php?tpl=search&q="+encodeURIComponent(searchvalue);
			}
		</script>
		<input type="text" id="abc" class="search-txt" value="<?php if(isset($_GET['q'])) echo $_GET['q'];?>" />
		<a class="search-btn" href="javascript:do_search();"></a>
	</div>
	<p class="line-t-15"></p>
	<div class="box nav">
		<div class="box nav-stair" id="nav-stair">
			<a href="<?php echo SITE_PATH;?>"<?php if($cid<=0 && !strstr($_SERVER['QUERY_STRING'],'special') && !strstr($_SERVER['QUERY_STRING'],'lastupdate') && !isset($topid)) echo 'class="n-sel"';?>>首页</a>
			<!-- 顶级分类 开始 -->
			<?php
					foreach($c->categories as $a){
						if($a['parent_id']=='0'){
							$style='';
							if((intval($cid)>0 && ($a['cate_id'] == $cid || $a['cate_id']==$c->categories[$cid]['parent_id'])) || (isset($topid) && $topid == $a['cate_id'])) $style='class="n-sel"';
							echo '<a href="'.$a['surl'].'"'.$style.'>'.$a['cname'].'</a>';
						}
					}
            ?>
			<a href="<?php echo $c->url->encode('special_list', array('host' => '/','p' => 1));?>" <?php if(strstr($_SERVER['REQUEST_URI'],'special')) echo 'class="n-sel"';?>>专题 </a>
			<!-- 顶级分类 结束 -->
		</div>
		<!-- 子分类 开始 -->
		<?php if($tpl !== 'index' && $cid!==''):?>

			<?php
                    if(intval($c->categories[$cid]['parent_id'])>0){
                        //echo('<a href="'.$c->categories[$c->categories[$cid]['parent_id']]['surl'].'" '.'>全部</a>');
                    }
                    if($c->categories[$cid]['parent_id']==0){
                        $a=$c->cate_son($cid);
                    }else{
                        $a=$c->cate_son($c->categories[$cid]['parent_id']);
                    }
					$style = '';
					if(!empty($a)){
						echo '<div class="nav-second">';
						foreach($a as $b){
							if($b['cate_id'] == $cid){
								$style='class="sd-sel"';
							}else{
								$style = '';
							}
							echo('<a href="'.$b['surl'].'" '.$style.'>'.$b['cname'].'</a>');
						}
						echo '</div>';
					}
            ?>
		<?php endif;?>
		<!-- 子分类 结束 -->
	</div>
</div>