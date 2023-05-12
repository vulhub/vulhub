<?php
if(!empty($_POST)) {
    $imap = @imap_open('{'.$_POST['hostname'].':993/imap/ssl}INBOX', $_POST['username'], $_POST['password']);
}
?>
<!doctype html>
<html lang="en">
<head>
    <!-- Required meta tags -->
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">

    <!-- Bootstrap CSS -->
    <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/bootstrap@4.1.3/dist/css/bootstrap.min.css" integrity="sha256-eSi1q2PG6J7g7ib17yAaWMcrr5GrtohYChqibrV7PBE=" crossorigin="anonymous">

    <title>Input your email server</title>
</head>
<body>

<div class="container">
        <h2>Test your email server</h2>
        <?php if(!empty($_POST)): ?>
        <?php if($imap): ?>
            <div class="alert alert-success" role="alert">
                Connect successful!
            </div>
        <?php else: ?>
            <div class="alert alert-danger" role="alert">
                Connect failed!
            </div>
        <?php endif; ?>
        <?php endif; ?>
        <form method="post">
            <div class="form-group">
                <label>Server address</label>
                <input type="text" name="hostname" class="form-control" >
            </div>
            <div class="form-group">
                <label>Username</label>
                <input type="text" name="username" class="form-control" >
            </div>
            <div class="form-group">
                <label>Password</label>
                <input type="password" class="form-control" name="password">
            </div>
            <button type="submit" class="btn btn-primary">Submit</button>
        </form>
</div>


<!-- Optional JavaScript -->
<!-- jQuery first, then Popper.js, then Bootstrap JS -->
<script src="https://cdn.jsdelivr.net/npm/jquery@3.3.1/dist/jquery.min.js" integrity="sha256-FgpCb/KJQlLNfOu91ta32o/NMZxltwRo8QtmkMRdAu8=" crossorigin="anonymous"></script>
<script src="https://cdn.jsdelivr.net/npm/popper.js@1.14.4/dist/umd/popper.min.js" integrity="sha256-EGs9T1xMHdvM1geM8jPpoo8EZ1V1VRsmcJz8OByENLA=" crossorigin="anonymous"></script>
<script src="https://cdn.jsdelivr.net/npm/bootstrap@4.1.3/dist/js/bootstrap.min.js" integrity="sha256-VsEqElsCHSGmnmHXGQzvoWjWwoznFSZc6hs7ARLRacQ=" crossorigin="anonymous"></script>
</body>
</html>