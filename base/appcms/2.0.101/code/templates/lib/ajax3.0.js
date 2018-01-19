//recvType 有两个值HTML 和 XML , 默认为HTML
function Ajax(recvType) {
	var aj = new Object();
	aj.targetUrl = '';    //请求的地址 可以是PHP也可以XML文件
	aj.sendString = '';   //请求服务器传递的字符串  ？ & 格式 url 

	aj.recvType=recvType ? recvType.toUpperCase() : 'HTML';//HTML XML
	aj.resultHandle = null;

	aj.createXMLHttpRequest = function() {
		var request = false;
		if(window.XMLHttpRequest) {
			request = new XMLHttpRequest();
			if(request.overrideMimeType) {
				request.overrideMimeType('text/xml');
			}
		} else if(window.ActiveXObject) {
			var versions = ['Microsoft.XMLHTTP', 'MSXML.XMLHTTP', 'Microsoft.XMLHTTP', 'Msxml2.XMLHTTP.7.0', 'Msxml2.XMLHTTP.6.0', 'Msxml2.XMLHTTP.5.0', 'Msxml2.XMLHTTP.4.0', 'MSXML2.XMLHTTP.3.0', 'MSXML2.XMLHTTP'];
			for(var i=0; i<versions.length; i++) {
				try {
					request = new ActiveXObject(versions[i]);
					if(request) {
						return request;
					}
				} catch(e) {
					request=false;
				}
			}
		}
		return request;
	}

	aj.XMLHttpRequest = aj.createXMLHttpRequest();

	aj.processHandle = function() {
		//alert(aj.XMLHttpRequest.responseText);
		//return;
		if(aj.XMLHttpRequest.readyState == 4) {
			if(aj.XMLHttpRequest.status == 200) {
				if(aj.recvType == 'HTML') {
					aj.resultHandle(aj.XMLHttpRequest.responseText);
				} else if(aj.recvType == 'XML') {
					aj.resultHandle(aj.XMLHttpRequest.responseXML);
				}
			}
		}
	}

	aj.get = function(targetUrl, resultHandle) {
		aj.targetUrl = targetUrl;
		if(resultHandle!=null){
			aj.XMLHttpRequest.onreadystatechange = aj.processHandle;
			aj.resultHandle = resultHandle;
		}
		if(window.XMLHttpRequest) {
			aj.XMLHttpRequest.open('GET', aj.targetUrl);
			aj.XMLHttpRequest.send(null);
		} else {
	        	aj.XMLHttpRequest.open("GET", targetUrl, true);
	        	aj.XMLHttpRequest.send();
		}
	}

	aj.post = function(targetUrl, sendString, resultHandle) {
		aj.targetUrl = targetUrl;

		if(typeof(sendString)=="object"){
			var str="";
			for(var pro in sendString){
				str+=pro+"="+sendString[pro]+"&";
			}
			
			aj.sendString=str.substr(0, str.length-1);
		}else{
			aj.sendString = sendString;
		}

	
		
		if(resultHandle!=null){
			aj.XMLHttpRequest.onreadystatechange = aj.processHandle;
			aj.resultHandle = resultHandle;
		}
		aj.XMLHttpRequest.open('POST', targetUrl);
		aj.XMLHttpRequest.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
		aj.XMLHttpRequest.send(aj.sendString); 
	}
	return aj;
}
