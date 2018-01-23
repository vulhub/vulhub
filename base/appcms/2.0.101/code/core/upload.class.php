<?php 
// +----------------------------------------------------------------------
// | ThinkPHP [ WE CAN DO IT JUST THINK IT ]
// +----------------------------------------------------------------------
// | Copyright (c) 2009 http://thinkphp.cn All rights reserved.
// +----------------------------------------------------------------------
// | Licensed ( http://www.apache.org/licenses/LICENSE-2.0 )
// +----------------------------------------------------------------------
// | Author: liu21st <liu21st@gmail.com>
// +----------------------------------------------------------------------
/**
 * 文件上传类
 * 
 * @category ORG
 * @package ORG
 * @subpackage Net
 * @author liu21st <liu21st@gmail.com> 
 */
class UploadFile { // 类定义开始
    private $config = array('maxSize' => -1, // 上传文件的最大值
        'supportMulti' => true, // 是否支持多文件上传
        'allowExts' => array(), // 允许上传的文件后缀 留空不作后缀检查
        'allowTypes' => array(), // 允许上传的文件类型 留空不做检查
        'thumb' => false, // 使用对上传图片进行缩略图处理
        'imageClassPath' => 'ORG.Util.Image', // 图库类包路径
        'thumbMaxWidth' => '', // 缩略图最大宽度
        'thumbMaxHeight' => '', // 缩略图最大高度
        'thumbPrefix' => 'thumb_', // 缩略图前缀
        'thumbSuffix' => '',
        'thumbPath' => '', // 缩略图保存路径
        'thumbFile' => '', // 缩略图文件名
        'thumbExt' => '', // 缩略图扩展名
        'thumbRemoveOrigin' => false, // 是否移除原图
        'zipImages' => false, // 压缩图片文件上传
        'autoSub' => false, // 启用子目录保存文件
        'subType' => 'hash', // 子目录创建方式 可以使用hash date custom
        'subDir' => '', // 子目录名称 subType为custom方式后有效
        'dateFormat' => 'Ymd',
        'hashLevel' => 1, // hash的目录层次
        'savePath' => '', // 上传文件保存路径
        'autoCheck' => true, // 是否自动检查附件
        'uploadReplace' => false, // 存在同名是否覆盖
        'saveRule' => 'uniqid', // 上传文件命名规则
        'hashType' => 'md5_file', // 上传文件Hash规则函数名
        ); 
    // 错误信息
    private $error = ''; 
    // 上传成功的文件信息
    private $uploadFileInfo ;

    public function __get($name) {
        if (isset($this -> config[$name])) {
            return $this -> config[$name];
        } 
        return null;
    } 

    public function __set($name, $value) {
        if (isset($this -> config[$name])) {
            $this -> config[$name] = $value;
        } 
    } 

    public function __isset($name) {
        return isset($this -> config[$name]);
    } 

    /**
     * 架构函数
     * 
     * @access public 
     * @param array $config 上传参数
     */
    public function __construct($config = array()) {
        if (is_array($config)) {
            $this -> config = array_merge($this -> config, $config);
        } 
    } 

    /**
     * 上传一个文件
     * 
     * @access public 
     * @param mixed $name 数据
     * @param string $value 数据表名
     * @return string 
     */
    private function save($file) {
        $filename = $file['savepath'] . $file['savename'];
        if (!$this -> uploadReplace && is_file($filename)) {
            // 不覆盖同名文件
            $this -> error = '文件已经存在！' . $filename;
            return false;
        } 
        // 如果是图像文件 检测文件格式
        if (in_array(strtolower($file['extension']), array('gif', 'jpg', 'jpeg', 'bmp', 'png', 'swf'))) {
            $info = getimagesize($file['tmp_name']);
            if (false === $info || ('gif' == strtolower($file['extension']) && empty($info['bits']))) {
                $this -> error = '非法图像文件';
                return false;
            } 
        } 
        if (!move_uploaded_file($file['tmp_name'], $this -> autoCharset($filename, 'utf-8', 'gbk'))) {
            $this -> error = '文件上传保存错误！';
            return false;
        } 
        if ($this -> thumb && in_array(strtolower($file['extension']), array('gif', 'jpg', 'jpeg', 'bmp', 'png'))) {
            $image = getimagesize($filename);
            if (false !== $image) {
                // 是图像文件生成缩略图
                $thumbWidth = explode(',', $this -> thumbMaxWidth);
                $thumbHeight = explode(',', $this -> thumbMaxHeight);
                $thumbPrefix = explode(',', $this -> thumbPrefix);
                $thumbSuffix = explode(',', $this -> thumbSuffix);
                $thumbFile = explode(',', $this -> thumbFile);
                $thumbPath = $this -> thumbPath?$this -> thumbPath:dirname($filename) . '/';
                $thumbExt = $this -> thumbExt ? $this -> thumbExt : $file['extension']; //自定义缩略图扩展名 
                // 生成图像缩略图
                for($i = 0, $len = count($thumbWidth); $i < $len; $i++) {
                    if (!empty($thumbFile[$i])) {
                        $thumbname = $thumbFile[$i];
                    } else {
                        $prefix = isset($thumbPrefix[$i])?$thumbPrefix[$i]:$thumbPrefix[0];
                        $suffix = isset($thumbSuffix[$i])?$thumbSuffix[$i]:$thumbSuffix[0];
                        $thumbname = $prefix . basename($filename, '.' . $file['extension']) . $suffix;
                    } 
                    Image :: thumb($filename, $thumbPath . $thumbname . '.' . $thumbExt, '', $thumbWidth[$i], $thumbHeight[$i], true);
                } 
                if ($this -> thumbRemoveOrigin) {
                    // 生成缩略图之后删除原图
                    unlink($filename);
                } 
            } 
        } 
        if ($this -> zipImags) {
            // TODO 对图片压缩包在线解压
        } 
        return true;
    } 

    /**
     * 上传所有文件
     * 
     * @access public 
     * @param string $savePath 上传文件保存路径
     * @return string 
     */
    public function upload($savePath = '') {
        // 如果不指定保存文件名，则由系统默认
        if (empty($savePath))
            $savePath = $this -> savePath; 
        // 检查上传目录
        if (!is_dir($savePath)) {
            // 检查目录是否编码后的
            if (is_dir(base64_decode($savePath))) {
                $savePath = base64_decode($savePath);
            } else {
                // 尝试创建目录
                if (!mkdir($savePath)) {
                    $this -> error = '上传目录' . $savePath . '不存在';
                    return false;
                } 
            } 
        } else {
            if (!is_writeable($savePath)) {
                $this -> error = '上传目录' . $savePath . '不可写';
                return false;
            } 
        } 
        $fileInfo = array();
        $isUpload = false; 
        // 获取上传的文件信息
        // 对$_FILES数组信息处理
        $files = $this -> dealFiles($_FILES);
        foreach($files as $key => $file) {
            // 过滤无效的上传
            if (!empty($file['name'])) {
                // 登记上传文件的扩展信息
                if (!isset($file['key'])) $file['key'] = $key;
                $file['extension'] = $this -> getExt($file['name']);
                $file['savepath'] = $savePath;
                $file['savename'] = $this -> getSaveName($file); 
                // 自动检查附件
                if ($this -> autoCheck) {
                    if (!$this -> check($file))
                        return false;
                } 
                // 保存上传文件
                if (!$this -> save($file)) return false;
                if (function_exists($this -> hashType)) {
                    $fun = $this -> hashType;
                    $file['hash'] = $fun($this -> autoCharset($file['savepath'] . $file['savename'], 'utf-8', 'gbk'));
                } 
                // 上传成功后保存文件信息，供其他地方调用
                unset($file['tmp_name'], $file['error']);
                $fileInfo[] = $file;
                $isUpload = true;
            } 
        } 
        if ($isUpload) {
            $this -> uploadFileInfo = $fileInfo;
            return true;
        } else {
            $this -> error = '没有选择上传文件';
            return false;
        } 
    } 

    /**
     * 上传单个上传字段中的文件 支持多附件
     * 
     * @access public 
     * @param array $file 上传文件信息
     * @param string $savePath 上传文件保存路径
     * @return string 
     */
    public function uploadOne($file, $savePath = '') {
        // 如果不指定保存文件名，则由系统默认
        if (empty($savePath))
            $savePath = $this -> savePath; 
        // 检查上传目录
        if (!is_dir($savePath)) {
            // 尝试创建目录
            if (!mkdir($savePath, 0777, true)) {
                $this -> error = '上传目录' . $savePath . '不存在';
                return false;
            } 
        } else {
            if (!is_writeable($savePath)) {
                $this -> error = '上传目录' . $savePath . '不可写';
                return false;
            } 
        } 
        // 过滤无效的上传
        if (!empty($file['name'])) {
            $fileArray = array();
            if (is_array($file['name'])) {
                $keys = array_keys($file);
                $count = count($file['name']);
                for ($i = 0; $i < $count; $i++) {
                    foreach ($keys as $key)
                    $fileArray[$i][$key] = $file[$key][$i];
                } 
            } else {
                $fileArray[] = $file;
            } 
            $info = array();
            foreach ($fileArray as $key => $file) {
                // 登记上传文件的扩展信息
                $file['extension'] = $this -> getExt($file['name']);
                $file['savepath'] = $savePath;
                $file['savename'] = $this -> getSaveName($file); 
                // 自动检查附件
                if ($this -> autoCheck) {
                    if (!$this -> check($file))
                        return false;
                } 
                // 保存上传文件
                if (!$this -> save($file)) return false;
                if (function_exists($this -> hashType)) {
                    $fun = $this -> hashType;
                    $file['hash'] = $fun($this -> autoCharset($file['savepath'] . $file['savename'], 'utf-8', 'gbk'));
                } 
                unset($file['tmp_name'], $file['error']);
                $info[] = $file;
            } 
            // 返回上传的文件信息
            return $info;
        } else {
            $this -> error = '没有选择上传文件';
            return false;
        } 
    } 

    /**
     * 转换上传文件数组变量为正确的方式
     * 
     * @access private 
     * @param array $files 上传的文件变量
     * @return array 
     */
    private function dealFiles($files) {
        $fileArray = array();
        $n = 0;
        foreach ($files as $key => $file) {
            if (is_array($file['name'])) {
                $keys = array_keys($file);
                $count = count($file['name']);
                for ($i = 0; $i < $count; $i++) {
                    $fileArray[$n]['key'] = $key;
                    foreach ($keys as $_key) {
                        $fileArray[$n][$_key] = $file[$_key][$i];
                    } 
                    $n++;
                } 
            } else {
                $fileArray[$key] = $file;
            } 
        } 
        return $fileArray;
    } 

    /**
     * 获取错误代码信息
     * 
     * @access public 
     * @param string $errorNo 错误号码
     * @return void 
     */
    protected function error($errorNo) {
        switch ($errorNo) {
            case 1:
                $this -> error = '上传的文件超过了 php.ini 中 upload_max_filesize 选项限制的值';
                break;
            case 2:
                $this -> error = '上传文件的大小超过了 HTML 表单中 MAX_FILE_SIZE 选项指定的值';
                break;
            case 3:
                $this -> error = '文件只有部分被上传';
                break;
            case 4:
                $this -> error = '没有文件被上传';
                break;
            case 6:
                $this -> error = '找不到临时文件夹';
                break;
            case 7:
                $this -> error = '文件写入失败';
                break;
            default:
                $this -> error = '未知上传错误！';
        } 
        return ;
    } 

    /**
     * 根据上传文件命名规则取得保存文件名
     * 
     * @access private 
     * @param string $filename 数据
     * @return string 
     */
    private function getSaveName($filename) {
        $rule = $this -> saveRule;
        if (empty($rule)) { // 没有定义命名规则，则保持文件名不变
            $saveName = $filename['name'];
        } else {
            if (function_exists($rule)) {
                // 使用函数生成一个唯一文件标识号
                $saveName = $rule() . "." . $filename['extension'];
            } else {
                // 使用给定的文件名作为标识号
                $saveName = $rule . "." . $filename['extension'];
            } 
        } 
        if ($this -> autoSub) {
            // 使用子目录保存文件
            $filename['savename'] = $saveName;
            $saveName = $this -> getSubName($filename) . $saveName;
        } 
        return $saveName;
    } 

    /**
     * 获取子目录的名称
     * 
     * @access private 
     * @param array $file 上传的文件信息
     * @return string 
     */
    private function getSubName($file) {
        switch ($this -> subType) {
            case 'custom':
                $dir = $this -> subDir;
                break;
            case 'date':
                $dir = date($this -> dateFormat, time()) . '/';
                break;
            case 'hash':
            default:
                $name = md5($file['savename']);
                $dir = '';
                for($i = 0;$i < $this -> hashLevel;$i++) {
                    $dir .= $name{$i} . '/';
                } 
                break;
        } 
        if (!is_dir($file['savepath'] . $dir)) {
            mkdir($file['savepath'] . $dir, 0777, true);
        } 
        return $dir;
    } 

    /**
     * 检查上传的文件
     * 
     * @access private 
     * @param array $file 文件信息
     * @return boolean 
     */
    private function check($file) {
        if ($file['error'] !== 0) {
            // 文件上传失败
            // 捕获错误代码
            $this -> error($file['error']);
            return false;
        } 
        // 文件上传成功，进行自定义规则检查
        // 检查文件大小
        if (!$this -> checkSize($file['size'])) {
            $this -> error = '上传文件大小不符！';
            return false;
        } 
        // 检查文件Mime类型
        if (!$this -> checkType($file['type'])) {
            $this -> error = '上传文件MIME类型不允许！';
            return false;
        } 
        // 检查文件类型
        if (!$this -> checkExt($file['extension'])) {
            $this -> error = '上传文件类型不允许';
            return false;
        } 
        // 检查是否合法上传
        if (!$this -> checkUpload($file['tmp_name'])) {
            $this -> error = '非法上传文件！';
            return false;
        } 
        return true;
    } 
    // 自动转换字符集 支持数组转换
    private function autoCharset($fContents, $from = 'gbk', $to = 'utf-8') {
        $from = strtoupper($from) == 'UTF8' ? 'utf-8' : $from;
        $to = strtoupper($to) == 'UTF8' ? 'utf-8' : $to;
        if (strtoupper($from) === strtoupper($to) || empty($fContents) || (is_scalar($fContents) && !is_string($fContents))) {
            // 如果编码相同或者非字符串标量则不转换
            return $fContents;
        } 
        if (function_exists('mb_convert_encoding')) {
            return mb_convert_encoding($fContents, $to, $from);
        } elseif (function_exists('iconv')) {
            return iconv($from, $to, $fContents);
        } else {
            return $fContents;
        } 
    } 

    /**
     * 检查上传的文件类型是否合法
     * 
     * @access private 
     * @param string $type 数据
     * @return boolean 
     */
    private function checkType($type) {
        if (!empty($this -> allowTypes))
            return in_array(strtolower($type), $this -> allowTypes);
        return true;
    } 

    /**
     * 检查上传的文件后缀是否合法
     * 
     * @access private 
     * @param string $ext 后缀名
     * @return boolean 
     */
    private function checkExt($ext) {
        if (!empty($this -> allowExts))
            return in_array(strtolower($ext), $this -> allowExts, true);
        return true;
    } 

    /**
     * 检查文件大小是否合法
     * 
     * @access private 
     * @param integer $size 数据
     * @return boolean 
     */
    private function checkSize($size) {
        return !($size > $this -> maxSize) || (-1 == $this -> maxSize);
    } 

    /**
     * 检查文件是否非法提交
     * 
     * @access private 
     * @param string $filename 文件名
     * @return boolean 
     */
    private function checkUpload($filename) {
        return is_uploaded_file($filename);
    } 

    /**
     * 取得上传文件的后缀
     * 
     * @access private 
     * @param string $filename 文件名
     * @return boolean 
     */
    private function getExt($filename) {
        $pathinfo = pathinfo($filename);
        return $pathinfo['extension'];
    } 

    /**
     * 取得上传文件的信息
     * 
     * @access public 
     * @return array 
     */
    public function getUploadFileInfo() {
        return $this -> uploadFileInfo;
    } 

    /**
     * 取得最后一次错误信息
     * 
     * @access public 
     * @return string 
     */
    public function getErrorMsg() {
        return $this -> error;
    } 
} 
