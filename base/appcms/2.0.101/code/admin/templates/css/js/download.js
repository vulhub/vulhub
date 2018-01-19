function set_src(){
    var start=$("#start").val();
    var end=$("#end").val();
    url='download_frame.php?m=list&s='+start+'&end='+end+'&now='+start;
    $("#downiframe").attr({"src":url});
}