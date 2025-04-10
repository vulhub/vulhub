@echo off
rem Licensed to the Apache Software Foundation (ASF) under one or more
rem contributor license agreements.  See the NOTICE file distributed with
rem this work for additional information regarding copyright ownership.
rem The ASF licenses this file to You under the Apache License, Version 2.0
rem (the "License"); you may not use this file except in compliance with
rem the License.  You may obtain a copy of the License at
rem
rem     http://www.apache.org/licenses/LICENSE-2.0
rem
rem Unless required by applicable law or agreed to in writing, software
rem distributed under the License is distributed on an "AS IS" BASIS,
rem WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
rem See the License for the specific language governing permissions and
rem limitations under the License.

rem This script creates the directory structure required for running Tomcat
rem in a separate directory by pointing %CATALINA_BASE% to it. It copies the
rem conf directory from %CATALINA_HOME%, and creates empty directories for
rem bin, lib, logs, temp, webapps, and work.
rem
rem If the file %CATALINA_HOME%/bin/setenv.sh exists then it is copied to
rem the target directory as well.
rem
rem Usage: makebase <path-to-target-directory> [-w | --webapps]

setlocal

rem Guess CATALINA_HOME if not defined
set "CURRENT_DIR=%cd%"
if not "%CATALINA_HOME%" == "" goto gotHome
set "CATALINA_HOME=%CURRENT_DIR%"
if exist "%CATALINA_HOME%\bin\catalina.bat" goto okHome
cd ..
set "CATALINA_HOME=%cd%"
cd "%CURRENT_DIR%"
:gotHome

if exist "%CATALINA_HOME%\bin\catalina.bat" goto okHome
echo The CATALINA_HOME environment variable is not defined correctly
echo This environment variable is needed to run this program
goto EOF
:okHome

rem first arg is the target directory
set BASE_TGT=%1

if %BASE_TGT%.==. (
    rem target directory not provided; exit
    echo Usage: makebase ^<path-to-target-directory^>
    goto :EOF
)

set COPY_WEBAPPS=false

rem parse args
for %%a in (%*) do (
   if "%%~a"=="--webapps" (
       set COPY_WEBAPPS=true
   )
   if "%%~a"=="-w" (
       set COPY_WEBAPPS=true
   )
)

if exist %BASE_TGT% (
  rem target directory exists
  echo Target directory exists

    rem exit if target directory is not empty
    for /F %%i in ('dir /b %BASE_TGT%\*.*') do (
        echo Target directory is not empty
        goto :EOF
    )
) else (
    rem create the target directory
    mkdir %BASE_TGT%
)

rem create empty directories
for %%d in (bin, conf, lib, logs, temp, webapps, work) do (
    mkdir %BASE_TGT%\%%d
)

if "%COPY_WEBAPPS%" == "true" (
    echo Copying webapps
    robocopy %CATALINA_HOME%\webapps %BASE_TGT%\webapps /E > nul
    rem copy conf directory recursively
    robocopy %CATALINA_HOME%\conf %BASE_TGT%\conf /E > nul
) else (
    rem copy conf directory without subdirectories and suppress warning
    robocopy %CATALINA_HOME%\conf %BASE_TGT%\conf > nul
    rem create empty ROOT directory
    mkdir %BASE_TGT%\webapps\ROOT
)

rem copy setenv.bat if exists
robocopy %CATALINA_HOME%\bin %BASE_TGT%\bin setenv.bat > nul

echo Created CATALINA_BASE directory at %BASE_TGT%

echo.
echo You can launch the new instance by running:
echo     set CATALINA_HOME=%CATALINA_HOME%
echo     set CATALINA_BASE=%BASE_TGT%
echo     %%CATALINA_HOME%%/bin/catalina.bat run

echo.
echo Attention: The ports in conf\server.xml might be bound by a
echo     different instance. Please review your config files
echo     and update them where necessary.
echo.

:EOF
