package org.pwntester.action;

import com.opensymphony.xwork2.ActionSupport;

public class IndexAction  extends ActionSupport {

    private String id;


    public String changeId(){
        return SUCCESS;
    }

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }
}
