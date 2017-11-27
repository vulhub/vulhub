<?php
require 'vendor/autoload.php';

if (isset($_POST['your-name'], $_POST['your-email'], $_POST['your-message'])) {
    $mail = new PHPMailer();
    $mail->isSMTP();
    $mail->Host = $_ENV['SMTP_SERVER'];
    $mail->SMTPAuth = true;
    $mail->Username = $_ENV['SMTP_EMAIL'];
    $mail->Password = $_ENV['SMTP_PASSWORD'];
    $mail->Port = intval($_ENV['SMTP_PORT']);
    $mail->CharSet = 'UTF-8';
    if (in_array($_ENV['SMTP_SECURE'], ['tls', 'ssl'])) {
        $mail->SMTPSecure = $_ENV['SMTP_SECURE'];
    } else {
        $mail->SMTPSecure = false;
    }

    $message = <<<DATA
<p><strong>{$_POST['your-name']}</strong>您好，您反馈的意见如下：</p>

<blackquote>{$_POST['your-message']}</blockquote>

<p>我们将在24小时内给予您回复，请耐心等待！</p>
DATA;
    $mail->SetFrom($_POST["your-email"], $_POST["your-name"]);
    $mail->AddAddress($_POST["your-email"], $_POST["your-name"]);
    $mail->Subject = "您反馈的意见我们已经收到";
    $mail->MsgHTML($message);
    if(!$mail->Send()) echo "Error: ".$mail->ErrorInfo; else echo "Success!";
}
?>
<html>
<head>
<meta charset="UTF-8">
</head>
<body>
<h2>反馈意见</h2>
<form method="post">
<p><label>昵称<br /><input type="text" name="your-name" value="" size="40" /></span> </label></p>
<p><label>Email<br /><input type="email" name="your-email" value="" size="40" /></span> </label></p>
<p><label>意见<br /><textarea name="your-message" cols="40" rows="10"></textarea></label></p>
<p><input type="submit" value="提交" /></p>
</body>
</html>