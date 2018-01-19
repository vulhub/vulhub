/*
Copyright (c) 2003-2011, CKSource - Frederico Knabben. All rights reserved.
For licensing, see LICENSE.html or http://ckeditor.com/license
*/

CKEDITOR.plugins.add('docprops',{init:function(a){var b=new CKEDITOR.dialogCommand('docProps');b.modes={wysiwyg:a.config.fullPage};a.addCommand('docProps',b);CKEDITOR.dialog.add('docProps',this.path+'dialogs/docprops.js');a.ui.addButton('DocProps',{label:a.lang.docprops.label,command:'docProps'});}});
