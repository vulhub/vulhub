/*
Copyright (c) 2003-2011, CKSource - Frederico Knabben. All rights reserved.
For licensing, see LICENSE.html or http://ckeditor.com/license
*/

(function(){var a=function(b){if(!b.window)return;var c=b.document,d=b.window.getViewPaneSize().height,e;if(CKEDITOR.env.ie||CKEDITOR.env.gecko)e=c.getBody().$.scrollHeight+(CKEDITOR.env.ie&&CKEDITOR.env.quirks?0:24);else e=c.getDocumentElement().$.offsetHeight;var f=b.config.autoGrow_minHeight,g=b.config.autoGrow_maxHeight;f==undefined&&(b.config.autoGrow_minHeight=f=200);if(f)e=Math.max(e,f);if(g)e=Math.min(e,g);if(e!=d){e=b.fire('autoGrow',{currentHeight:d,newHeight:e}).newHeight;b.resize(b.container.getStyle('width'),e,true);}};CKEDITOR.plugins.add('autogrow',{init:function(b){for(var c in {contentDom:1,key:1,selectionChange:1,insertElement:1})b.on(c,function(d){var e=b.getCommand('maximize');if(d.editor.mode=='wysiwyg'&&(!e||e.state!=CKEDITOR.TRISTATE_ON))setTimeout(function(){a(d.editor);},100);});}});})();
