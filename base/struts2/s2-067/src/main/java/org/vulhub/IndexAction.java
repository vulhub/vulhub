/*
 * Copyright 2006 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package org.vulhub;

import com.opensymphony.xwork2.ActionSupport;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import org.apache.commons.io.FileUtils;
import org.apache.struts2.ServletActionContext;
import org.apache.logging.log4j.LogManager;

public class IndexAction extends ActionSupport {
    private File file;
    private String fileContentType;
    private String fileFileName;
    private String uploadedFilePath;
    private String uploadedFileName;

    public String execute() throws Exception {
        if (file != null) {
            String base = ServletActionContext.getServletContext().getRealPath("/") + "upload";
            LogManager.getLogger(IndexAction.class).debug("Upload base directory: {}", base);
            // Create upload directory if it doesn't exist
            File dir = new File(base);
            if (!dir.exists()) {
                dir.mkdirs();
            }

            // Save the uploaded file
            String fileName = getFileFileName();
            File destFile = new File(dir, fileName);
            FileUtils.copyFile(getFile(), destFile);
            
            // Save the path and filename for display
            uploadedFilePath = "upload/" + fileName;
            uploadedFileName = getFileFileName();
            return SUCCESS;
        }
        return INPUT;
    }

    // Getters and setters
    public File getFile() {
        return file;
    }

    public void setFile(File file) {
        this.file = file;
    }

    public String getFileContentType() {
        return fileContentType;
    }

    public void setFileContentType(String fileContentType) {
        this.fileContentType = fileContentType;
    }

    public String getFileFileName() {
        return fileFileName;
    }

    public void setFileFileName(String fileFileName) {
        this.fileFileName = fileFileName;
    }

    public String getUploadedFilePath() {
        return uploadedFilePath;
    }

    public void setUploadedFilePath(String uploadedFilePath) {
        this.uploadedFilePath = uploadedFilePath;
    }

    public String getUploadedFileName() {
        return uploadedFileName;
    }

    public void setUploadedFileName(String uploadedFileName) {
        this.uploadedFileName = uploadedFileName;
    }
}
