<!DOCTYPE html PUBLIC 
	"-//W3C//DTD XHTML 1.1 Transitional//EN"
	"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
	
<%@taglib prefix="s" uri="/struts-tags" %>

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
	<title>File Upload</title>
	<s:head />
	<style>
		.form-container {
			max-width: 500px;
			margin: 50px auto;
			padding: 20px;
			border: 1px solid #ccc;
			border-radius: 5px;
		}
		.form-field {
			margin-bottom: 15px;
		}
		.submit-button {
			background-color: #4CAF50;
			color: white;
			padding: 10px 20px;
			border: none;
			border-radius: 4px;
			cursor: pointer;
		}
		.submit-button:hover {
			background-color: #45a049;
		}
	</style>
</head>
<body>
	<div class="form-container">
		<h2>File Upload</h2>
		<s:form action="index" method="post" enctype="multipart/form-data">
			<div class="form-field">
				<s:file label="Select a file" name="file" />
			</div>
			<s:submit value="Upload" cssClass="submit-button" />
		</s:form>
	</div>
</body>
</html>
	