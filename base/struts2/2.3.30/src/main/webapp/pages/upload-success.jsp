<%@ taglib prefix="s" uri="/struts-tags" %>
<html>
<head>
   <title>Struts2 Showcase - Fileupload sample</title>
</head>

<body>
<div class="page-header">
   <h1>Fileupload sample</h1>
</div>

<div class="container-fluid">
   <div class="row-fluid">
      <div class="span12">
         <s:actionerror cssClass="alert alert-error"/>
         <s:fielderror cssClass="alert alert-error"/>
         <s:form action="doUpload" method="POST" enctype="multipart/form-data">
            <s:file name="upload" label="File"/>
            <s:textfield name="caption" label="Caption"/>
            <s:submit cssClass="btn btn-primary"/>
         </s:form>
      </div>
   </div>
</div>
</body>
</html>

