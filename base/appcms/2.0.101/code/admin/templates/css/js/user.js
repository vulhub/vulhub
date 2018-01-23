/* 修改用户状态 */
function edit_ustate(uid, ustate) {
    $.post("admin_user.php?ajax=json&m=edit", {
        "uid": uid,
        "ustate": ustate
    }, function(result) {
        try {
            var ret = $.evalJSON(result);
            ret.code == 0 ? (C.alert.tips({
                "content": "" + ret.msg + ""
            }), setTimeout(function() {
                window.location.href = "admin_user.php";
            }, 1500)) : C.alert.tips({
                "content": "" + ret.msg + ""
            });
        } catch (e) {
            alert(result + ":e");
        }
    });
}
/* 添加账号弹出层 */
function add_user_show() {
    C.form.init([ ".ipt" ]);
    C.alert.opacty({
        "width": "295",
        "height": "195",
        "title": "添加管理账号",
        "div_tag": "#html_user"
    });
}
/* 编辑账号资料 */
function user_edit() {
    var reg = /^[A-Za-z][A-Za-z]*[a-z0-9_]*$/, uid = $("#uid").val(), uname = $("#uname").val(), upass = $("#upass").val(), re_pass = $("#re_pass").val();
    $.post("admin_user.php?ajax=json&m=edit", {
        "uid": uid,
        "uname": uname,
        "upass": upass,
        "re_pass": re_pass
    }, function(result) {
        try {
            var ret = $.evalJSON(result);
            ret.code == 0 ? (C.alert.tips({"content":""+ret.msg+""}), setTimeout(function() {
                window.location.href = "admin_user.php";
            }, 1500)) : C.alert.tips({"content":""+ret.msg+""});
        } catch (e) {
            alert(result + ":e");
        }
    });
}