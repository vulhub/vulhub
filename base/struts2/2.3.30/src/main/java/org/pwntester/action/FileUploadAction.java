package org.pwntester.action;

import com.opensymphony.xwork2.ActionSupport;
import java.io.File;

public class FileUploadAction extends ActionSupport {

    private String contentType;
    private File upload;
    private String fileName;
    private String caption;

    public String input() throws Exception {
        return SUCCESS;
    }


    public String upload() throws Exception {
        return SUCCESS;
    }

    public String getUploadFileName() {
        return fileName;
    }

    public void setUploadFileName(String fileName) {
        this.fileName = fileName;
    }

    public String getUploadContentType() {
        return contentType;
    }

    public void setUploadContentType(String contentType) {
        this.contentType = contentType;
    }

    public File getUpload() {
        return upload;
    }

    public void setUpload(File upload) {
        this.upload = upload;
    }

    public String getCaption() {
        return caption;
    }

    public void setCaption(String caption) {
        this.caption = caption;
    }
}


