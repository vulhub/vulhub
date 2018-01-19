<!-- 左侧菜单开始 -->
<div class="left_nav">
    <?php 
        //计算选中菜单
        $is_set=0;
        for($i=0;$i<count($menu);$i++){
            $sonmenu=$menu[$i]['sonmenu'];
            for($j=0;$j<count($sonmenu);$j++){
                $show = check_url($sonmenu[$j]['url']);
                if($show){
                    $is_set=1;
                    $menu[$i]['selected'] = $show;
                    $menu[$i]['sonmenu'][$j]['selected']=$show;
                }
            }
        }
        if($is_set==0){
            $menu[3]['selected'] = 1;
            //$menu[0]['sonmenu'][0]['selected']=1;
        }
        //显示菜单
        echo('<ul>');
        foreach($menu as $a){
            $selected=isset($a['selected'])?'selected':'';
            echo('<li>');
            echo('<a href="'.$a['sonmenu'][0]['url'].'" class="menu_1'.$selected.'" style="background:url('.$a['bgimg'].') 12px 0 no-repeat;">'.$a['title'].'</a>');
            if($selected=='selected'){
                foreach($a['sonmenu'] as $b){
                    $selected=isset($b['selected'])?'selected':'';
                    $target=($b['title']=='我的账号')?' target="_blank"':'';
                    echo('<a href="'.$b['url'].'" class="menu_2'.$selected.'"'.$target.'>'.$b['title'].'</a>'); 
                }
            }
            echo('</li>');
        }
        echo('</ul>');
    ?>
    
</div><!-- 左侧菜单结束 -->