/*
Copyright (c) 2003-2011, CKSource - Frederico Knabben. All rights reserved.
For licensing, see LICENSE.html or http://ckeditor.com/license
*/

CKEDITOR.editorConfig = function( config )
{
	// Define changes to default configuration here. For example:
	// config.uiColor = '#AADC6E';
	config.language = 'zh-cn';//中文
    config.font_names = '宋体;楷体_GB2312;新宋体;黑体;隶书;幼圆;微软雅黑;Arial;Comic Sans MS;Courier New;Tahoma;Times New Roman;Verdana';
	config.enterMode = CKEDITOR.ENTER_BR;
	config.extraPlugins = "autoformat";
	//config.extraPlugins = 'up_files';
};
