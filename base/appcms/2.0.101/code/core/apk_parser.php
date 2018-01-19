<?php
/******************************************************
 * Android APK File Parser
 * Author: Katana
 * Version: v0.1
 * Web: http://www.win-ing.cn
 *
 * 功能：解析安卓apk包中的压缩XML文件，还原和读取XML内容
 *
 * 依赖功能：需要PHP的ZIP包函数支持。
 ******************************************************/
 
class ApkParser{
//----------------------
// 公共函数，供外部调用
//----------------------
    public function open($apk_file, $xml_file='AndroidManifest.xml'){
        $zip = new ZipArchive;
        if ($zip->open($apk_file) === TRUE) {
            $xml = $zip->getFromName($xml_file);
            $zip->close();
            if ($xml){
                try {
                    return $this->parseString($xml);
                }catch (Exception $e){
                }
            }
        }
        return false;
    }
 
    public function parseString($xml){
        $this->xml = $xml;
        $this->length = strlen($xml);
 
        $this->root = $this->parseBlock(self::AXML_FILE);
        return true;
    }
 
    public function getXML($node=NULL, $lv=-1){
        if ($lv == -1) $node = $this->root;
        if (!$node) return '';
 
        if ($node['type'] == self::END_TAG) $lv--;
        $xml = ($node['line'] == 0 || $node['line'] == $this->line) ? '' : "\n".str_repeat('  ', $lv);
        $xml .= $node['tag'];
        $this->line = $node['line'];
        foreach ($node['child'] as $c){
            $xml .= $this->getXML($c, $lv+1);
        }
        return $xml;
    }
 
    public function getPackage(){
        return $this->getAttribute('manifest', 'package');
    }
 
    public function getVersionName(){
        return $this->getAttribute('manifest', 'android:versionName');
    }
 
    public function getVersionCode(){
        return $this->getAttribute('manifest', 'android:versionCode');
    }
 
    public function getAppName(){
        return $this->getAttribute('manifest/application', 'android:name');
    }
 
    public function getMainActivity(){
        for ($id=0; true; $id++){
            $act = $this->getAttribute("manifest/application/activity[{$id}]/intent-filter/action", 'android:name');
            if (!$act) break;
            if ($act == 'android.intent.action.MAIN') return $this->getActivity($id);
        }
        return NULL;
    }
 
    public function getActivity($idx=0){
        $idx = intval($idx);
        return $this->getAttribute("manifest/application/activity[{$idx}]", 'android:name');
    }
 
    public function getAttribute($path, $name){
        $r = $this->getElement($path);
        if (is_null($r)) return NULL;
 
        if (isset($r['attrs'])){
            foreach ($r['attrs'] as $a){
                if ($a['ns_name'] == $name) return $this->getAttributeValue($a);
            }
        }
        return NULL;
    }
 
//----------------------
// 类型常量定义
//----------------------
    const AXML_FILE             = 0x00080003;
    const STRING_BLOCK          = 0x001C0001;
    const RESOURCEIDS           = 0x00080180;
    const START_NAMESPACE       = 0x00100100;
    const END_NAMESPACE         = 0x00100101;
    const START_TAG             = 0x00100102;
    const END_TAG               = 0x00100103;
    const TEXT                  = 0x00100104;
 
    const TYPE_NULL             =0;
    const TYPE_REFERENCE        =1;
    const TYPE_ATTRIBUTE        =2;
    const TYPE_STRING           =3;
    const TYPE_FLOAT            =4;
    const TYPE_DIMENSION        =5;
    const TYPE_FRACTION         =6;
    const TYPE_INT_DEC          =16;
    const TYPE_INT_HEX          =17;
    const TYPE_INT_BOOLEAN      =18;
    const TYPE_INT_COLOR_ARGB8  =28;
    const TYPE_INT_COLOR_RGB8   =29;
    const TYPE_INT_COLOR_ARGB4  =30;
    const TYPE_INT_COLOR_RGB4   =31;
 
    const UNIT_MASK             = 15;
    private static $RADIX_MULTS = array(0.00390625, 3.051758E-005, 1.192093E-007, 4.656613E-010);
    private static $DIMENSION_UNITS = array("px","dip","sp","pt","in","mm","","");
    private static $FRACTION_UNITS  = array("%","%p","","","","","","");
 
    private $xml='';
    private $length = 0;
    private $stringCount = 0;
    private $styleCount  = 0;
    private $stringTab = array();
    private $styleTab  = array();
    private $resourceIDs = array();
    private $ns = array();
    private $cur_ns = NULL;
    private $root = NULL;
    private $line = 0;
 
//----------------------
// 内部私有函数
//----------------------
    private function getElement($path){
        if (!$this->root) return NULL;
        $ps = explode('/', $path);
        $r  = $this->root;
        foreach ($ps as $v){
            if (preg_match('/([^\[]+)\[([0-9]+)\]$/', $v, $ms)){
                $v = $ms[1];
                $off = $ms[2];
            }else {
                $off = 0;
            }
            foreach ($r['child'] as $c){
                if ($c['type'] == self::START_TAG && $c['ns_name'] == $v){
                    if ($off == 0){
                        $r = $c; continue 2;
                    }else {
                        $off--;
                    }
                }
            }
            // 没有找到节点
            return NULL;
        }
        return $r;
    }
 
    private function parseBlock($need = 0){
        $o = 0;
        $type = $this->get32($o);
        if ($need && $type != $need) throw new Exception('Block Type Error', 1);
        $size = $this->get32($o);
        if ($size < 8 || $size > $this->length) throw new Exception('Block Size Error', 2);
        $left = $this->length - $size;
 
        $props = false;
        switch ($type){
            case self::AXML_FILE:
                $props = array(
                    'line' => 0,
                    'tag' => '<?xml version="1.0" encoding="utf-8"?>'
                );
            break;
            case self::STRING_BLOCK:
                $this->stringCount = $this->get32($o);
                $this->styleCount  = $this->get32($o);
                $o += 4;
                $strOffset = $this->get32($o);
                $styOffset = $this->get32($o);
                $strListOffset = $this->get32array($o, $this->stringCount);
                $styListOffset = $this->get32array($o, $this->styleCount);
                $this->stringTab = $this->stringCount > 0 ? $this->getStringTab($strOffset, $strListOffset) : array();
                $this->styleTab  = $this->styleCount > 0 ? $this->getStringTab($styOffset, $styListOffset) : array();
                $o = $size;
            break;
            case self::RESOURCEIDS:
                $count = $size / 4 - 2;
                $this->resourceIDs = $this->get32array($o, $count);
            break;
            case self::START_NAMESPACE:
                $o += 8;
                $prefix = $this->get32($o);
                $uri = $this->get32($o);
 
                if (empty($this->cur_ns)){
                    $this->cur_ns = array();
                    $this->ns[] = &$this->cur_ns;
                }
                $this->cur_ns[$uri] = $prefix;
            break;
            case self::END_NAMESPACE:
                $o += 8;
                $prefix = $this->get32($o);
                $uri = $this->get32($o);
 
                if (empty($this->cur_ns)) break;
                unset($this->cur_ns[$uri]);
            break;
            case self::START_TAG:
                $line = $this->get32($o);
 
                $o += 4;
                $attrs = array();
                $props = array(
                    'line' => $line,
                    'ns' => $this->getNameSpace($this->get32($o)),
                    'name' => $this->getString($this->get32($o)),
                    'flag' => $this->get32($o),
                    'count' => $this->get16($o),
                    'id' => $this->get16($o)-1,
                    'class' => $this->get16($o)-1,
                    'style' => $this->get16($o)-1,
                    'attrs' => &$attrs
                );
                $props['ns_name'] = $props['ns'].$props['name'];
                for ($i=0; $i < $props['count']; $i++){
                    $a = array(
                        'ns' => $this->getNameSpace($this->get32($o)),
                        'name' => $this->getString($this->get32($o)),
                        'val_str' => $this->get32($o),
                        'val_type' => $this->get32($o),
                        'val_data' => $this->get32($o)
                    );
                    $a['ns_name'] = $a['ns'].$a['name'];
                    $a['val_type'] >>= 24;
                    $attrs[] = $a;
                }
                // 处理TAG字符串
                $tag = "<{$props['ns_name']}";
                foreach ($this->cur_ns as $uri => $prefix){
                    $uri = $this->getString($uri);
                    $prefix = $this->getString($prefix);
                    $tag .= " xmlns:{$prefix}=\"{$uri}\"";
                }
                foreach ($props['attrs'] as $a){
                    $tag .= " {$a['ns_name']}=\"".
                            $this->getAttributeValue($a).
                            '"';
                }
                $tag .= '>';
                $props['tag'] = $tag;
 
                unset($this->cur_ns);
                $this->cur_ns = array();
                $this->ns[] = &$this->cur_ns;
                $left = -1;
            break;
            case self::END_TAG:
                $line = $this->get32($o);
                $o += 4;
                $props = array(
                    'line' => $line,
                    'ns' => $this->getNameSpace($this->get32($o)),
                    'name' => $this->getString($this->get32($o))
                );
                $props['ns_name'] = $props['ns'].$props['name'];
                $props['tag'] = "</{$props['ns_name']}>";
                if (count($this->ns) > 1){
                    array_pop($this->ns);
                    unset($this->cur_ns);
                    $this->cur_ns = array_pop($this->ns);
                    $this->ns[] = &$this->cur_ns;
                }
            break;
            case self::TEXT:
                $o += 8;
                $props = array(
                    'tag' => $this->getString($this->get32($o))
                );
                $o += 8;
            break;
            default:
                throw new Exception('Block Type Error', 3);
            break;
        }
 
        $this->skip($o);
        $child = array();
        while ($this->length > $left){
            $c = $this->parseBlock();
            if ($props && $c) $child[] = $c;
            if ($left == -1 && $c['type'] == self::END_TAG){
                $left = $this->length;
                break;
            }
        }
        if ($this->length != $left) throw new Exception('Block Overflow Error', 4);
        if ($props){
            $props['type'] = $type;
            $props['size'] = $size;
            $props['child'] = $child;
            return $props;
        }else {
            return false;
        }
    }
 
    private function getAttributeValue($a){
        $type = &$a['val_type'];
        $data = &$a['val_data'];
        switch ($type){
            case self::TYPE_STRING:
                return $this->getString($a['val_str']);
            case self::TYPE_ATTRIBUTE:
                return sprintf('?%s%08X', self::_getPackage($data), $data);
            case self::TYPE_REFERENCE:
                return sprintf('@%s%08X', self::_getPackage($data), $data);
            case self::TYPE_INT_HEX:
                return sprintf('0x%08X', $data);
            case self::TYPE_INT_BOOLEAN:
                return ($data != 0 ? 'true' : 'false');
            case self::TYPE_INT_COLOR_ARGB8:
            case self::TYPE_INT_COLOR_RGB8:
            case self::TYPE_INT_COLOR_ARGB4:
            case self::TYPE_INT_COLOR_RGB4:
                return sprintf('#%08X', $data);
            case self::TYPE_DIMENSION:
                return $this->_complexToFloat($data).self::$DIMENSION_UNITS[$data & self::UNIT_MASK];
            case self::TYPE_FRACTION:
                return $this->_complexToFloat($data).self::$FRACTION_UNITS[$data & self::UNIT_MASK];
            case self::TYPE_FLOAT:
                return $this->_int2float($data);
        }
        if ($type >=self::TYPE_INT_DEC && $type < self::TYPE_INT_COLOR_ARGB8){
            return (string)$data;
        }
        return sprintf('<0x%X, type 0x%02X>', $data, $type);
    }
 
    private function _complexToFloat($data){
        return (float)($data & 0xFFFFFF00) * self::$RADIX_MULTS[($data>>4) & 3];
    }
    private function _int2float($v) {
        $x = ($v & ((1 << 23) - 1)) + (1 << 23) * ($v >> 31 | 1);
        $exp = ($v >> 23 & 0xFF) - 127;
        return $x * pow(2, $exp - 23);
    }
    private static function _getPackage($data){
        return ($data >> 24 == 1) ? 'android:' : '';
    }
 
    private function getStringTab($base, $list){
        $tab = array();
        foreach ($list as $off){
            $off += $base;
            $len = $this->get16($off);
            $mask = ($len >> 0x8) & 0xFF;
            $len = $len & 0xFF;
            if ($len == $mask){
                if ($off + $len > $this->length) throw new Exception('String Table Overflow', 11);
                $tab[] = substr($this->xml, $off, $len);
            }else {
                if ($off + $len * 2 > $this->length) throw new Exception('String Table Overflow', 11);
                $str = substr($this->xml, $off, $len * 2);
                $tab[] = mb_convert_encoding($str, 'UTF-8', 'UCS-2LE');
            }
        }
        return $tab;
    }
    private function getString($id){
        if ($id > -1 && $id < $this->stringCount){
            return $this->stringTab[$id];
        }else {
            return '';
        }
    }
    private function getNameSpace($uri){
        for ($i=count($this->ns); $i > 0; ){
            $ns = $this->ns[--$i];
            if (isset($ns[$uri])){
                $ns = $this->getString($ns[$uri]);
                if (!empty($ns)) $ns .= ':';
                return $ns;
            }
        }
        return '';
    }
    private function get32(&$off){
        $int = unpack('V', substr($this->xml, $off, 4));
        $off += 4;
        return array_shift($int);
    }
    private function get32array(&$off, $size){
        if ($size <= 0) return NULL;
        $arr = unpack('V*', substr($this->xml, $off, 4 * $size));
        if (count($arr) != $size) throw new Exception('Array Size Error', 10);
        $off += 4 * $size;
        return $arr;
    }
    private function get16(&$off){
        $int = unpack('v', substr($this->xml, $off, 2));
        $off += 2;
        return array_shift($int);
    }
    private function skip($size){
        $this->xml = substr($this->xml, $size);
        $this->length -= $size;
    }
}
?>