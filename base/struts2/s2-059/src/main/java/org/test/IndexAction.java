package org.test;

import com.opensymphony.xwork2.ActionSupport;

public class IndexAction extends ActionSupport {
    private String skillName;
    private String url;

    public String getSkillName() {
        return skillName;
    }

    public void setSkillName(String skillName) {
        this.skillName = skillName;
    }

    public String getUrl() {
        return url;
    }

    public void setUrl(String url) {
        this.url = url;
    }

    @Override
    public String execute() throws Exception {
        System.out.println(skillName);
        System.out.println(url);
        return "success";
    }
}
