(function(){
//Section 1 : 按下自定义按钮时执行的代码
var a= {
exec: function(editor) {
            editor.insertHtml("[nextpage]");
        }
},
//Section 2 : 创建自定义按钮、绑定方法
 b = 'nextpage';
    CKEDITOR.plugins.add(b, {
        init: function(editor) {
            editor.addCommand(b, a);
            editor.ui.addButton('nextpage', {
                label: '插入分页代码，上一页，下一页',
                icon: this.path + 'nextpage.gif',
                command: b
            });
        }
    });
})();
