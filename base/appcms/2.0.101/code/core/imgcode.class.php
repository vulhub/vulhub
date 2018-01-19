<?php
class Image { // 类定义开始
    /**
     * +----------------------------------------------------------
     * 取得图像信息
     * 
     * +----------------------------------------------------------
     * 
     * @static 
     * @access public +----------------------------------------------------------
     * @param string $image 图像文件名
+----------------------------------------------------------
     * @return mixed +----------------------------------------------------------
     */
    static function getImageInfo($img) {
        $imageInfo = getimagesize($img);
        if ($imageInfo !== false) {
            $imageType = strtolower(substr(image_type_to_extension($imageInfo[2]), 1));
            $imageSize = filesize($img);
            $info = array("width" => $imageInfo[0],
                "height" => $imageInfo[1],
                "type" => $imageType,
                "size" => $imageSize,
                "mime" => $imageInfo['mime']
                );
            return $info;
        } else {
            return false;
        } 
    } 

    /**
     * +----------------------------------------------------------
     * 生成图像验证码
+----------------------------------------------------------
     * 
     * @static 
     * @access public +----------------------------------------------------------
     * @param string $length 位数
     * @param string $mode 类型
     * @param string $type 图像格式
     * @param string $width 宽度
     * @param string $height 高度
     * +----------------------------------------------------------
     * @return string +----------------------------------------------------------
     */
    static function buildImageVerify($length = 4, $mode = 1, $type = 'png', $width = 48, $height = 22, $verifyName = 'verify') {
        // tsload(ADDON_PATH.'/liberary/String.class.php');
        $randval = String :: rand_string($length, $mode); 
        // 转换成大写字母.
        // $_SESSION['login_o']=strtoupper($randval);
        $_SESSION[$verifyName] = md5(strtoupper($randval));
        $width = ($length * 10 + 10) > $width?$length * 10 + 10:$width;
        if ($type != 'gif' && function_exists('imagecreatetruecolor')) {
            $im = @imagecreatetruecolor($width, $height);
        } else {
            $im = @imagecreate($width, $height);
        } 
        $r = Array(225, 255, 255, 223);
        $g = Array(225, 236, 237, 255);
        $b = Array(225, 236, 166, 125);
        $key = mt_rand(0, 3);

        $backColor = imagecolorallocate($im, $r[$key], $g[$key], $b[$key]); //背景色（随机）
        $borderColor = imagecolorallocate($im, 100, 100, 100); //边框色
        $pointColor = imagecolorallocate($im, mt_rand(0, 255), mt_rand(0, 255), mt_rand(0, 255)); //点颜色
        @imagefilledrectangle($im, 0, 0, $width - 1, $height - 1, $backColor);
        @imagerectangle($im, 0, 0, $width-1, $height-1, $borderColor);
        $stringColor = imagecolorallocate($im, mt_rand(0, 200), mt_rand(0, 120), mt_rand(0, 120)); 
        // 干扰
        for($i = 0;$i < 10;$i++) {
            $fontcolor = imagecolorallocate($im, mt_rand(0, 255), mt_rand(0, 255), mt_rand(0, 255));
            imagearc($im, mt_rand(-10, $width), mt_rand(-10, $height), mt_rand(30, 300), mt_rand(20, 200), 55, 44, $fontcolor);
        } 
        for($i = 0;$i < 25;$i++) {
            $fontcolor = imagecolorallocate($im, mt_rand(0, 255), mt_rand(0, 255), mt_rand(0, 255));
            imagesetpixel($im, mt_rand(0, $width), mt_rand(0, $height), $pointColor);
        } 
        for($i = 0;$i < $length;$i++) {
            imagestring($im, 5, $i * 10 + 5, mt_rand(1, 8), $randval{$i}, $stringColor);
        } 
        //  @imagestring ($im, 5, 5, 3, $randval, $stringColor);
        Image :: output($im, $type);
    } 
    // 中文验证码
    static function GBVerify($length = 4, $type = 'png', $width = 120, $height = 30, $fontface = 'simhei.ttf', $verifyName = 'verify') {
        // tsload(ADDON_PATH.'/liberary/String.class.php');
        $code = String :: rand_string($length, 4);
        $width = ($length * 45) > $width?$length * 45:$width;
        $_SESSION[$verifyName] = md5($code);
        $im = imagecreatetruecolor($width, $height);
        $borderColor = imagecolorallocate($im, 100, 100, 100); //边框色
        $bkcolor = imagecolorallocate($im, 250, 250, 250);
        imagefill($im, 0, 0, $bkcolor);
        @imagerectangle($im, 0, 0, $width-1, $height-1, $borderColor); 
        // 干扰
        for($i = 0;$i < 15;$i++) {
            $fontcolor = imagecolorallocate($im, mt_rand(0, 255), mt_rand(0, 255), mt_rand(0, 255));
            imagearc($im, mt_rand(-10, $width), mt_rand(-10, $height), mt_rand(30, 300), mt_rand(20, 200), 55, 44, $fontcolor);
        } 
        for($i = 0;$i < 255;$i++) {
            $fontcolor = imagecolorallocate($im, mt_rand(0, 255), mt_rand(0, 255), mt_rand(0, 255));
            imagesetpixel($im, mt_rand(0, $width), mt_rand(0, $height), $fontcolor);
        } 
        if (!is_file($fontface)) {
            $fontface = dirname(__FILE__) . "/" . $fontface;
        } 
        for($i = 0;$i < $length;$i++) {
            $fontcolor = imagecolorallocate($im, mt_rand(0, 120), mt_rand(0, 120), mt_rand(0, 120)); //这样保证随机出来的颜色较深。
            $codex = String :: msubstr($code, $i, 1);
            imagettftext($im, mt_rand(16, 20), mt_rand(-60, 60), 40 * $i + 20, mt_rand(30, 35), $fontcolor, $fontface, $codex);
        } 
        Image :: output($im, $type);
    } 

    static function output($im, $type = 'png', $filename = '') {
        header("Content-type: image/" . $type);
        $ImageFun = 'image' . $type;
        if (empty($filename)) {
            $ImageFun($im);
        } else {
            $ImageFun($im, $filename);
        } 
        imagedestroy($im);
    } 
} //类定义结束
class String {
    /**
     * +----------------------------------------------------------
     * 生成UUID 单机使用
     * +----------------------------------------------------------
     * 
     * @access public +----------------------------------------------------------
     * @return string +----------------------------------------------------------
     */
    static public function uuid() {
        $charid = md5(uniqid(mt_rand(), true));
        $hyphen = chr(45); // "-"
        $uuid = chr(123)// "{"
         . substr($charid, 0, 8) . $hyphen
         . substr($charid, 8, 4) . $hyphen
         . substr($charid, 12, 4) . $hyphen
         . substr($charid, 16, 4) . $hyphen
         . substr($charid, 20, 12)
         . chr(125); // "}"
        return $uuid;
    } 

    /**
     * +----------------------------------------------------------
     * 生成Guid主键
     * +----------------------------------------------------------
     * 
     * @return Boolean +----------------------------------------------------------
     */
    static public function keyGen() {
        return str_replace('-', '', substr(com_create_guid(), 1, -1));
    } 

    /**
     * +----------------------------------------------------------
     * 检查字符串是否是UTF8编码
     * +----------------------------------------------------------
     * 
     * @param string $string 字符串
+----------------------------------------------------------
     * @return Boolean +----------------------------------------------------------
     */
    function is_utf8($str) {
        $c = 0;
        $b = 0;
        $bits = 0;
        $len = strlen($str);
        for($i = 0; $i < $len; $i++) {
            $c = ord($str[$i]);
            if ($c > 128) {
                if (($c >= 254)) return false;
                elseif ($c >= 252) $bits = 6;
                elseif ($c >= 248) $bits = 5;
                elseif ($c >= 240) $bits = 4;
                elseif ($c >= 224) $bits = 3;
                elseif ($c >= 192) $bits = 2;
                else return false;
                if (($i + $bits) > $len) return false;
                while ($bits > 1) {
                    $i++;
                    $b = ord($str[$i]);
                    if ($b < 128 || $b > 191) return false;
                    $bits--;
                } 
            } 
        } 
        return true;
    } 

    /**
     * +----------------------------------------------------------
     * 字符串截取，支持中文和其它编码
+----------------------------------------------------------
     * 
     * @static 
     * @access public +----------------------------------------------------------
     * @param string $str 需要转换的字符串
     * @param string $start 开始位置
     * @param string $length 截取长度
     * @param string $charset 编码格式
     * @param string $suffix 截断显示字符
     * +----------------------------------------------------------
     * @return string +----------------------------------------------------------
     */
    static public function msubstr($str, $start = 0, $length, $charset = "utf-8", $suffix = true) {
        if (function_exists("mb_substr"))
            return mb_substr($str, $start, $length, $charset);
        elseif (function_exists('iconv_substr')) {
            return iconv_substr($str, $start, $length, $charset);
        } 
        $re['utf-8'] = "/[\x01-\x7f]|[\xc2-\xdf][\x80-\xbf]|[\xe0-\xef][\x80-\xbf]{2}|[\xf0-\xff][\x80-\xbf]{3}/";
        $re['gb2312'] = "/[\x01-\x7f]|[\xb0-\xf7][\xa0-\xfe]/";
        $re['gbk'] = "/[\x01-\x7f]|[\x81-\xfe][\x40-\xfe]/";
        $re['big5'] = "/[\x01-\x7f]|[\x81-\xfe]([\x40-\x7e]|\xa1-\xfe])/";
        preg_match_all($re[$charset], $str, $match);
        $slice = join("", array_slice($match[0], $start, $length));
        if ($suffix) return $slice . "…";
        return $slice;
    } 

    /**
     * +----------------------------------------------------------
     * 产生随机字串，可用来自动生成密码
     * 默认长度6位 字母和数字混合 支持中文
     * +----------------------------------------------------------
     * 
     * @param string $len 长度
     * @param string $type 字串类型
     * 0 字母 1 数字 其它 混合
     * @param string $addChars 额外字符
     * +----------------------------------------------------------
     * @return string +----------------------------------------------------------
     */
    static public function rand_string($len = 6, $type = '', $addChars = '') {
        $str = '';
        switch ($type) {
            case 0:
                $chars = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz' . $addChars;
                break;
            case 1:
                $chars = str_repeat('0123456789', 3);
                break;
            case 2:
                $chars = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ' . $addChars;
                break;
            case 3:
                $chars = 'abcdefghijklmnopqrstuvwxyz' . $addChars;
                break;
            case 4:
                $chars = "们以我到他会作时要动国产的一是工就年阶义发成部民可出能方进在了不和有大这主中人上为来分生对于学下级地个用同行面说种过命度革而多子后自社加小机也经力线本电高量长党得实家定深法表着水理化争现所二起政三好十战无农使性前等反体合斗路图把结第里正新开论之物从当两些还天资事队批点育重其思与间内去因件日利相由压员气业代全组数果期导平各基或月毛然如应形想制心样干都向变关问比展那它最及外没看治提五解系林者米群头意只明四道马认次文通但条较克又公孔领军流入接席位情运器并飞原油放立题质指建区验活众很教决特此常石强极土少已根共直团统式转别造切九你取西持总料连任志观调七么山程百报更见必真保热委手改管处己将修支识病象几先老光专什六型具示复安带每东增则完风回南广劳轮科北打积车计给节做务被整联步类集号列温装即毫知轴研单色坚据速防史拉世设达尔场织历花受求传口断况采精金界品判参层止边清至万确究书术状厂须离再目海交权且儿青才证低越际八试规斯近注办布门铁需走议县兵固除般引齿千胜细影济白格效置推空配刀叶率述今选养德话查差半敌始片施响收华觉备名红续均药标记难存测士身紧液派准斤角降维板许破述技消底床田势端感往神便贺村构照容非搞亚磨族火段算适讲按值美态黄易彪服早班麦削信排台声该击素张密害侯草何树肥继右属市严径螺检左页抗苏显苦英快称坏移约巴材省黑武培著河帝仅针怎植京助升王眼她抓含苗副杂普谈围食射源例致酸旧却充足短划剂宣环落首尺波承粉践府鱼随考刻靠够满夫失包住促枝局菌杆周护岩师举曲春元超负砂封换太模贫减阳扬江析亩木言球朝医校古呢稻宋听唯输滑站另卫字鼓刚写刘微略范供阿块某功套友限项余倒卷创律雨让骨远帮初皮播优占死毒圈伟季训控激找叫云互跟裂粮粒母练塞钢顶策双留误础吸阻故寸盾晚丝女散焊功株亲院冷彻弹错散商视艺灭版烈零室轻血倍缺厘泵察绝富城冲喷壤简否柱李望盘磁雄似困巩益洲脱投送奴侧润盖挥距触星松送获兴独官混纪依未突架宽冬章湿偏纹吃执阀矿寨责熟稳夺硬价努翻奇甲预职评读背协损棉侵灰虽矛厚罗泥辟告卵箱掌氧恩爱停曾溶营终纲孟钱待尽俄缩沙退陈讨奋械载胞幼哪剥迫旋征槽倒握担仍呀鲜吧卡粗介钻逐弱脚怕盐末阴丰雾冠丙街莱贝辐肠付吉渗瑞惊顿挤秒悬姆烂森糖圣凹陶词迟蚕亿矩康遵牧遭幅园腔订香肉弟屋敏恢忘编印蜂急拿扩伤飞露核缘游振操央伍域甚迅辉异序免纸夜乡久隶缸夹念兰映沟乙吗儒杀汽磷艰晶插埃燃欢铁补咱芽永瓦倾阵碳演威附牙芽永瓦斜灌欧献顺猪洋腐请透司危括脉宜笑若尾束壮暴企菜穗楚汉愈绿拖牛份染既秋遍锻玉夏疗尖殖井费州访吹荣铜沿替滚客召旱悟刺脑措贯藏敢令隙炉壳硫煤迎铸粘探临薄旬善福纵择礼愿伏残雷延烟句纯渐耕跑泽慢栽鲁赤繁境潮横掉锥希池败船假亮谓托伙哲怀割摆贡呈劲财仪沉炼麻罪祖息车穿货销齐鼠抽画饲龙库守筑房歌寒喜哥洗蚀废纳腹乎录镜妇恶脂庄擦险赞钟摇典柄辩竹谷卖乱虚桥奥伯赶垂途额壁网截野遗静谋弄挂课镇妄盛耐援扎虑键归符庆聚绕摩忙舞遇索顾胶羊湖钉仁音迹碎伸灯避泛亡答勇频皇柳哈揭甘诺概宪浓岛袭谁洪谢炮浇斑讯懂灵蛋闭孩释乳巨徒私银伊景坦累匀霉杜乐勒隔弯绩招绍胡呼痛峰零柴簧午跳居尚丁秦稍追梁折耗碱殊岗挖氏刃剧堆赫荷胸衡勤膜篇登驻案刊秧缓凸役剪川雪链渔啦脸户洛孢勃盟买杨宗焦赛旗滤硅炭股坐蒸凝竟陷枪黎救冒暗洞犯筒您宋弧爆谬涂味津臂障褐陆啊健尊豆拔莫抵桑坡缝警挑污冰柬嘴啥饭塑寄赵喊垫丹渡耳刨虎笔稀昆浪萨茶滴浅拥穴覆伦娘吨浸袖珠雌妈紫戏塔锤震岁貌洁剖牢锋疑霸闪埔猛诉刷狠忽灾闹乔唐漏闻沈熔氯荒茎男凡抢像浆旁玻亦忠唱蒙予纷捕锁尤乘乌智淡允叛畜俘摸锈扫毕璃宝芯爷鉴秘净蒋钙肩腾枯抛轨堂拌爸循诱祝励肯酒绳穷塘燥泡袋朗喂铝软渠颗惯贸粪综墙趋彼届墨碍启逆卸航衣孙龄岭骗休借" . $addChars;
                break;
            default : 
                // 默认去掉了容易混淆的字符oOLl和数字01，要添加请使用addChars参数
                $chars = 'ABCDEFGHIJKMNPQRSTUVWXYZabcdefghijkmnpqrstuvwxyz23456789' . $addChars;
                break;
        } 
        if ($len > 10) { // 位数过长重复字符串一定次数
            $chars = $type == 1? str_repeat($chars, $len) : str_repeat($chars, 5);
        } 
        if ($type != 4) {
            $chars = str_shuffle($chars);
            $str = substr($chars, 0, $len);
        } else {
            // 中文随机字
            for($i = 0;$i < $len;$i++) {
                $str .= self :: msubstr($chars, floor(mt_rand(0, mb_strlen($chars, 'utf-8')-1)), 1);
            } 
        } 
        return $str;
    } 

    /**
     * +----------------------------------------------------------
     * 生成一定数量的随机数，并且不重复
+----------------------------------------------------------
     * 
     * @param integer $number 数量
     * @param string $len 长度
     * @param string $type 字串类型
     * 0 字母 1 数字 其它 混合
     * +----------------------------------------------------------
     * @return string +----------------------------------------------------------
     */
    static public function build_count_rand ($number, $length = 4, $mode = 1) {
        if ($mode == 1 && $length < strlen($number)) {
            // 不足以生成一定数量的不重复数字
            return false;
        } 
        $rand = array();
        for($i = 0; $i < $number; $i++) {
            $rand[] = rand_string($length, $mode);
        } 
        $unqiue = array_unique($rand);
        if (count($unqiue) == count($rand)) {
            return $rand;
        } 
        $count = count($rand) - count($unqiue);
        for($i = 0; $i < $count * 3; $i++) {
            $rand[] = rand_string($length, $mode);
        } 
        $rand = array_slice(array_unique ($rand), 0, $number);
        return $rand;
    } 

    /**
     * +----------------------------------------------------------
     *    带格式生成随机字符 支持批量生成
     *    但可能存在重复
+----------------------------------------------------------
     * 
     * @param string $format 字符格式
     *       # 表示数字 * 表示字母和数字 $ 表示字母
     * @param integer $number 生成数量
     * +----------------------------------------------------------
     * @return string | array
     * +----------------------------------------------------------
     */
    static public function build_format_rand($format, $number = 1) {
        $str = array();
        $length = strlen($format);
        for($j = 0; $j < $number; $j++) {
            $strtemp = '';
            for($i = 0; $i < $length; $i++) {
                $char = substr($format, $i, 1);
                switch ($char) {
                    case "*":// 字母和数字混合
                        $strtemp .= String :: rand_string(1);
                        break;
                    case "#":// 数字
                        $strtemp .= String :: rand_string(1, 1);
                        break;
                    case "$":// 大写字母
                        $strtemp .= String :: rand_string(1, 2);
                        break;
                    default:// 其它格式均不转换
                        $strtemp .= $char;
                        break;
                } 
            } 
            $str[] = $strtemp;
        } 

        return $number == 1? $strtemp : $str ;
    } 

    /**
     * +----------------------------------------------------------
     * 获取一定范围内的随机数字 位数不足补零
     * +----------------------------------------------------------
     * 
     * @param integer $min 最小值
     * @param integer $max 最大值
+----------------------------------------------------------
     * @return string +----------------------------------------------------------
     */
    static public function rand_number ($min, $max) {
        return sprintf("%0" . strlen($max) . "d", mt_rand($min, $max));
    } 
} 

?>