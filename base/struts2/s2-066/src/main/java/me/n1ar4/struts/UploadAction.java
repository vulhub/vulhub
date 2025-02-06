package me.n1ar4.struts;

import com.opensymphony.xwork2.ActionSupport;
import org.apache.commons.io.FileUtils;
import org.apache.struts2.ServletActionContext;

import java.io.File;

public class UploadAction extends ActionSupport {
    private static final long serialVersionUID = 1L;
    private File upload;
    private String uploadContentType;
    private String uploadFileName;

    public File getUpload() {
        return upload;
    }

    public void setUpload(File upload) {
        this.upload = upload;
    }

    public String getUploadContentType() {
        return uploadContentType;
    }

    public void setUploadContentType(String uploadContentType) {
        this.uploadContentType = uploadContentType;
    }

    public String getUploadFileName() {
        return uploadFileName;
    }

    public void setUploadFileName(String uploadFileName) {
        this.uploadFileName = uploadFileName;
    }

    public String doUpload() {
        String path = ServletActionContext.getServletContext().getRealPath("/") + "upload";
        String realPath = path + File.separator + uploadFileName;
        try {
            FileUtils.copyFile(upload, new File(realPath));
            setFilePath(realPath);
            ServletActionContext.getRequest().setAttribute("filePath", uploadFileName);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return SUCCESS;
    }

    private String filePath;

    public String getFilePath() {
        return filePath;
    }

    public void setFilePath(String filePath) {
        this.filePath = filePath;
    }
}


