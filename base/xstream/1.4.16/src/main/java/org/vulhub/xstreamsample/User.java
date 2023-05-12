package org.vulhub.xstreamsample;


import com.thoughtworks.xstream.annotations.XStreamAlias;

@XStreamAlias("user")
public class User {
    protected String name;
    protected Integer age;

    public User(String name, Integer age) {
        this.name = name;
        this.age = age;
    }

    public String getName() {
        return name;
    }

    public Integer getAge() {
        return age;
    }

    public void setName(String name) {
        this.name = name;
    }

    public void setAge(Integer age) {
        this.age = age;
    }
}
