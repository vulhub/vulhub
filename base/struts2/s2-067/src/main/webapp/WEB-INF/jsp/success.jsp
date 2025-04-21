<!DOCTYPE html PUBLIC 
	"-//W3C//DTD XHTML 1.1 Transitional//EN"
	"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
	
<%@taglib prefix="s" uri="/struts-tags" %>

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
	<title>Upload Success</title>
	<style>
		.result-container {
			max-width: 500px;
			margin: 50px auto;
			padding: 20px;
			border: 1px solid #ccc;
			border-radius: 5px;
			text-align: center;
		}
		.file-info {
			margin: 20px 0;
			padding: 15px;
			background-color: #f5f5f5;
			border-radius: 4px;
		}
		.file-name {
			font-size: 18px;
			color: #333;
			margin-bottom: 10px;
			word-break: break-all;
		}
		.file-link {
			display: inline-block;
			margin-top: 10px;
			color: #4CAF50;
			text-decoration: none;
		}
		.file-link:hover {
			text-decoration: underline;
		}
		.back-button {
			background-color: #4CAF50;
			color: white;
			padding: 10px 20px;
			text-decoration: none;
			border-radius: 4px;
			display: inline-block;
			margin-top: 20px;
		}
		.back-button:hover {
			background-color: #45a049;
		}
	</style>
</head>
<body>
	<div class="result-container">
		<h2>Upload Success!</h2>
		<div class="file-info">
			<div class="file-name">
				File name: <s:property value="uploadedFileName" />
			</div>
			<a href="<s:property value="uploadedFilePath"/>" class="file-link" target="_blank">Download File</a>
		</div>
		<div>
			<a href="index.action" class="back-button">Upload Another File</a>
		</div>
	</div>
</body>
</html> 