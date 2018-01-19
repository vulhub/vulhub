/**
 *应用管理js文件
 */
/*选项卡切换*/
$(document).ready(function () {
    C.tabs({"params": [
        {"nav": "#tabbtn01", "con": "#tab001", "sclass": "current", "nclass": ""},
        {"nav": "#tabbtn02", "con": "#tab002", "sclass": "current", "nclass": ""},
        {"nav": "#tabbtn03", "con": "#tab003", "sclass": "current", "nclass": ""},
		{"nav": "#tabbtn04", "con": "#tab004", "sclass": "current", "nclass": ""}
    ]
    });
});
/*显示添加或者是编辑层*/
function show_edit(app_id) {
    if (app_id > 0) {
        $.post("app.php?ajax=json&m=list", {
            "app_id": app_id
        }, function (result) {
            try {
                var ret = $.evalJSON(result);
                var hist = ret.apps.list[0].history;
                var resource = ret.apps.list[0].resource;
                editor1.document.getBody().setHtml(ret.apps.list[0].app_desc);
                $("#app_id").val(ret.apps.list[0].app_id);
                if (hist.length > 0) {
                    $("#history_id").val(ret.apps.list[0].history[0].history_id);
                }
                $("#app_title").val(ret.apps.list[0].app_title);
                $("#app_stitle").val(ret.apps.list[0].app_stitle);
                $("#last_cate_id").find("option[value='" + ret.apps.list[0].last_cate_id + "']").attr("selected", true);
                $("#app_version").val(ret.apps.list[0].app_version);
                $("#app_size").val(ret.apps.list[0].app_size);
                $("#app_system").val(ret.apps.list[0].app_system);
                $("#app_type").val(ret.apps.list[0].app_type);
                $("#show_img").val(ret.apps.list[0].app_logo);
                $("#app_tags").val(ret.apps.list[0].app_tags);
                $("#app_logo").val(ret.apps.list[0].app_logo);
                $("#charge_app_id").val(ret.apps.list[0].charge_app_id);
                $("#rewrite_tag").val(ret.apps.list[0].rewrite_tag);
                if (ret.apps.list[0].app_logo != '') {
                    $("#showcimg").html("<img src='" + ret.apps.list[0].app_logo + "' width=24 height=24/>");
                }
                $("#app_grade").val(ret.apps.list[0].app_grade);
                $("#app_recomment").val(ret.apps.list[0].app_recomment);
                $("#app_comments").val(ret.apps.list[0].app_comments);
                $("#app_visitors").val(ret.apps.list[0].app_visitors);
                $("#last_cate_id").attr("disabled", true);
                /*显示历史版本信息*/
                $("#history_list").html('');
                //alert(hist.length);
                if (hist.length > 0) {
                    // his_html = "";
                    for (z = 0; z < hist.length; z++) {
                        //his_html += '<tr><td id="his_'+z+hist[z].app_version+'">'+hist[z].app_version+'</td><td id="his_'+z+hist[z].app_size+'">'+hist[z].app_size+'</td><td id="his_'+z+hist[z].appcms_history_id+'">'+hist[z].appcms_history_id+'</td><td id="his_'+z+hist[z].app_system+'">'+hist[z].app_system+'</td><td id="his_'+z+hist[z].app_system+'"><a href="javascript:show_history('+hist[z].history_id+','+ret.apps.list[0].app_id+')"></a></td></tr>';
                        var his_html = history_data(hist, ret.apps.list[0].app_id);
                        //alert(his_html);
                        $("#history_list").html(his_html);
                    }
                }

                /*显示资源信息*/
                if (resource.length > 0) {
                    $("#reso_list").html(show_resource(resource));
                }
				/*显示SEO信息*/
				$("#seo_title").val(ret.apps.list[0].seo_title);
				$("#seo_keywords").val(ret.apps.list[0].seo_keywords);
				$("#seo_desc").val(ret.apps.list[0].seo_desc);
                C.alert.opacty({
                    "width": "900",
                    "height": "530",
                    "title": "编辑应用",
                    "div_tag": "#appedit"
                });
            } catch (e) {
                alert(e.message + "::" + result);
            }
        });
    } else {
        C.form.init([ ".ipt", "#app_version_desc", ["#app_id", "0"], ["#history_id", "0"]]);
		$("#seo_desc").val("");
		$("#history_list").html("");
		$("#reso_list").html("");
        $("#showcimg").html("");
        $("#mode_up_box").html("");
        C.alert.opacty({"width": "900", "height": "530", "div_tag": "#appedit", "title": "添加应用"});
        editor1.document.getBody().setHtml("");
    }
}

/*显示历史版本*/
function history_data($history, $app_id) {
    var his_html = '';
    for (z = 0; z < $history.length; z++) {
        if ($history[z].down_url == '' && $history[z].appcms_history_id > 0) {
            his_html += '<tr>';
            his_html += '<td  class="alignleft" width="50">' + $history[z].app_version + '</td> ';
            his_html += '<td  class="alignleft" width="50">' + $history[z].app_size + '</td>';
            his_html += '<td  class="alignleft" width="50">' + $history[z].packname + '</td>';
            his_html += '<td  class="alignleft" width="50">云端应用</td>';
            his_html += '<td  class="alignleft" width="50">' + $history[z].app_system + '</td>';
            his_html += '<td style="text-align:left;"></td></tr>';
        } else {
            his_html += '<tr>';
            his_html += '<td  class="alignleft" width="50"><input type="text" size="50"  style="width:150px" class="ipt hiswidth"   id="' + $history[z].history_id + '_' + z + 'app_version"  name="app_version" value="' + $history[z].app_version + '"/></td> ';
            his_html += '<td  class="alignleft" width="50"><input type="text" size="50" style="width:150px" class="ipt hiswidth"  id="' + $history[z].history_id + '_' + z + 'app_size"   name="app_size" value="' + $history[z].app_size + '"/></td>';
            //his_html += '<td  class="alignleft" width="50"><input type="text" style="width:150px" class="ipt hiswidth"  name="app_apk" id="'+$history[z].history_id+'_'+z+'appcms_history_id" value="'+$history[z].appcms_history_id+'"/></td>';
            his_html += '<td  class="alignleft" width="50"><input type="text" size="50" style="width:150px" class="ipt hiswidth"  id="' + $history[z].history_id + '_' + z + 'packname"   name="packname" value="' + $history[z].packname + '"/></td>';
            /*
             if($history[z].down_url == '' && $history[z].appcms_history_id > 0){
             his_html += '<td  class="alignleft" width="50">云端应用</td>';
             } else {
             his_html += '<td  class="alignleft" width="50"><input type="text" size="50" style="width:150px" class="ipt hiswidth"  id="'+$history[z].history_id+'_'+z+'down_url"   name="app_system" value="'+$history[z].down_url+'"/></td>';
             }*/
            his_html += '<td  class="alignleft" width="50"><input type="text" size="50" style="width:150px" class="ipt hiswidth"  id="' + $history[z].history_id + '_' + z + 'down_url"   name="down_url" value="' + $history[z].down_url + '"/></td>';
            his_html += '<td  class="alignleft" width="50"><input type="text" size="50" style="width:150px" class="ipt hiswidth"  id="' + $history[z].history_id + '_' + z + 'app_system"   name="app_system" value="' + $history[z].app_system + '"/></td>';
            his_html += '<td style="text-align:left;"><a href="javascript:void(0);" class="but2" style="margin-right:0;" onclick="edithistory(' + $history[z].history_id + ',' + z + ',' + $app_id + ');">修改</a>&nbsp;<a href="javascript:void(0);" class="but2" style="margin-right:0;" onclick="his_del(' + $history[z].history_id + ');">删除</a></td></tr>';

        }
    }
    return his_html;
}


/* 根据资源id删除资源里面的资源，物理删除不可恢复*/
function delse(id, url) {
    if (!confirm("确认删除吗？")) {
        return false;
    }
    $.post("app.php?m=del_resource", {
        "id": id,
        "url": url
    }, function (result, textStatus) {
        try {
            var ret = $.evalJSON(result);
            ret.code == 0 ? (C.alert.tips({"content": "" + ret.msg + ""}), setTimeout(function () {
                $("#re_" + id).remove();
            }, 1500)) : C.alert.tips({"content": "" + ret.msg + ""});
        } catch (e) {
            alert(e.message + "::" + result);
        }
    });
}
/* 添加编辑应用资料*/
function app_edit() {
    var href = window.location.href;
    href = href.replace("jsfun=add", "");
    //var app_desc = editor1.document.getBody().getHtml();
    var app_desc = editor1.getData();
    var formvalues = C.form.get_form("appform");
    formvalues["app_desc"] = encodeURIComponent(app_desc);

    //获取上传图片框
    var resoc = [];
    $(".iptss").each(function (i) {
        resoc.push($(".iptss")[i].value);
    });
    formvalues['reso'] = resoc.join("#");

    $.post("app.php?m=save_edit", formvalues, function (result) {
        try {
            var ret = $.evalJSON(result), msg = ret.msg;
            ret.code == 0 ? (C.alert.tips({"content": "" + msg + ""}), setTimeout(function () {
                window.location.href = href;
            }, 1500)) : C.alert.tips({"content": "" + msg + ""});
        } catch (e) {
            alert(result + e.message);
        }
    });
}
/*删除选中*/
function app_del(app_id) {
    var href = window.location.href;
    href = href.replace("jsfun=add", "");
    if (!confirm("如果删除，会将该应用下的历史版本一起删除，确认删除？")) return;
    $.post("app.php?ajax=json&m=del", {
        "app_id": app_id
    }, function (result) {
        try {
            var ret = $.evalJSON(result), msg = ret.msg;
            if (ret.code == 0) {
                C.alert.tips({"content": "" + ret.msg + "", "timeout": "2000"});
                setTimeout(function () {
                    window.location.href = href;
                }, 1000);
            } else {
                C.alert.tips({"content": "" + ret.msg + ""});
            }
        } catch (e) {
            alert(result + e.message);
        }
    });
}
/*显示添加历史版本的界面*/
function show_history(history_id, app_id) {
    if (history_id <= 0) {
        C.form.init([ ".ipt", ["#down_url_his", "http://"]]);
        $("#his_app_id").val(app_id);
        $("#history_id_his").val(history_id);
        $("#app_version_desc_his").val("");
        C.alert.opacty({"width": "420", "height": "300", "div_tag": "#history", "title": "添加历史版本"});
    } else {
        $.post("app.php?m=history&ajax=json",
            {"history_id": history_id, "app_id": app_id}, function (result) {
                try {
                    var ret = $.evalJSON(result);
                    $("#his_app_id").val(ret.his.list[0].app_id),
                        $("#history_id_his").val(ret.his.list[0].history_id),
                        $("#app_version_his").val(ret.his.list[0].app_version),
                        $("#app_size_his").val(ret.his.list[0].app_size),
                        $("#app_system_his").val(ret.his.list[0].app_system),
                        $("#app_version_desc_his").val(ret.his.list[0].app_version_desc),
                        $("#appcms_history_id_his").val(ret.his.list[0].appcms_history_id),
                        $("#down_url_his").val(ret.his.list[0].down_url),
                        C.alert.opacty({"width": "320", "height": "320", "div_tag": "#history", "title": "修改历史版本"});
                } catch (e) {
                    alert("::" + result);
                }
            });
    }
}
/*添加历史版本*/
function app_history_edit() {
    var href = window.location.href;
    href = href.replace("jsfun=add", "");
    var formvalues = C.form.get_form("historyform");

    $.post("app.php?m=history_edit", formvalues, function (result) {
        try {
            var ret = $.evalJSON(result), msg = ret.msg;
            ret.code == 0 ? (C.alert.tips({"content": "" + msg + ""}), setTimeout(function () {
                window.location.reload();
            }, 1500)) : C.alert.tips({"content": "" + msg + ""});
        } catch (e) {
            alert("::" + result);
        }
    });
}
/*提交修改一个历史版本*/
function edithistory($his_id, $z, $app_id) {
    var tag = $his_id + '_' + $z;
    var app_version = $("#" + tag + "app_version").val();
    var app_size = $("#" + tag + "app_size").val();
    //var appcms_history_id = $("#"+tag+"appcms_history_id").val();
    var down_url = $("#" + tag + "down_url").val();
    var app_system = $("#" + tag + "app_system").val();
    var packname = $("#" + tag + "packname").val();
    //var data = {"app_version_his":app_version,"app_size_his":app_size,"appcms_history_id_his":appcms_history_id,"down_url_his":down_url,"app_system_his":app_system,"history_id_his":$his_id,"his_app_id":$app_id};
    var data = {"app_version_his": app_version, "packname": packname, "app_size_his": app_size, "down_url_his": down_url, "app_system_his": app_system, "history_id_his": $his_id, "his_app_id": $app_id};
    //alert($.toJSON(data)); return;
    $.post("app.php?m=history_edit", data, function (result) {
        try {
            var ret = $.evalJSON(result), msg = ret.msg;
            C.alert.tips({"content": "" + msg + ""});
        } catch (e) {
            alert("::" + result);
        }
    });
}
/*删除历史版本*/
function his_del(history_id) {
    var href = window.location.href;
    href = href.replace("jsfun=add", "");
    if (!confirm("确定删除？")) return;
    $.post("app.php?ajax=json&m=his_del", {
        "history_id": history_id
    }, function (result) {
        try {
            var ret = $.evalJSON(result), msg = ret.msg;
            if (ret.code == 0) {
                C.alert.tips({"content": "" + ret.msg + "", "timeout": "2000"});
                setTimeout(function () {
                    window.location.href = href;
                }, 1000);
            } else {
                C.alert.tips({"content": "" + ret.msg + ""});
            }
        } catch (e) {
            alert(result + e.message);
        }
    });
}

/* 提交查找 */
function search_submit(url) {
    var search_type = $("#search_type").val();
    var search_txt = $("#search_txt").val();
    var cate_id = $("#cate_id").val();
    try {
        if (cate_id == 0) {
            if (search_txt == '') {
                C.alert.tips({"content": "查找内容不能为空"});
                $('#search_txt').focus();
                return false;
            }
        }
        var url = url + '&search_type=' + search_type + '&cate_id=' + cate_id + '&search_txt=' + encodeURI(search_txt);
        window.location.href = url;
    } catch (e) {
        alert(e.message);
    }
}
/* 图片上传回调函数*/
function callback_upload_thumb(ret) {
    try {
        //alert(ret);
        var json = $.evalJSON(ret);
        if (json.files.length <= 0) {
            alert('上传失败');
            return false;
        }
        $("#app_logo").val(json.files[0].original);
        $("#showimg").html('<img src="' + json.files[0].original + '" width="30" height="30" >');
    } catch (e) {
        alert('err:' + e.message);
    }
}
/* apk上传回调函数*/
function callback_upload_apk(ret) {
    try {
        //alert(ret);return false;
        var json = $.evalJSON(ret);
        if (json.files.length <= 0) {
            alert('上传失败');
            return false;
        }
        $("#down_url_his").val(json.files[0].server + json.files[0].original);
        $("#app_version_his").val(json.files[0].version);
        $("#app_system_his").val(json.files[0].minsys);
        $("#app_size_his").val(json.files[0].file_size);
        $("#packname_his").val(json.files[0].packname);
    } catch (e) {
        alert('err:' + e.message);
    }
}

function get_tags(title) {
    $.post("app.php?m=get_tags&t=" + Math.random(), {"title": title}, function (data) {
        try {
            var tags = $.evalJSON(data);
            var app_tags = [];
            for (var i = 0; i < tags.length; i++) {
                app_tags.push(tags[i].k);
            }
            if ($.trim($("#app_tags").val()) == '') {
                $("#app_tags").val(app_tags.join(","));
            } else {
                if (confirm("确定要替换为新获取的标签吗？")) {
                    $("#app_tags").val(app_tags.join(","));
                }
            }
        } catch (e) {
            alert(e.message);
        }
    });
}

