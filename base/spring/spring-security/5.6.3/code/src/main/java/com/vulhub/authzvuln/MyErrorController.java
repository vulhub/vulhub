package com.vulhub.authzvuln;


import org.springframework.stereotype.Controller;
import org.springframework.boot.web.servlet.error.ErrorController;
import org.springframework.web.bind.annotation.RequestMapping;
import javax.servlet.http.HttpServletRequest;
import org.springframework.http.HttpStatus;
import javax.servlet.RequestDispatcher;


@Controller
public class MyErrorController implements ErrorController  {

@RequestMapping("/error")
public String handleError(HttpServletRequest request) {
	    Object status = request.getAttribute(RequestDispatcher.ERROR_STATUS_CODE);
	    
	    if (status != null) {
	        Integer statusCode = Integer.valueOf(status.toString());
	    
	        if(statusCode == HttpStatus.FORBIDDEN.value()) {
	            return "error-403";
	        }
	    }
	    return "error";
    }
}