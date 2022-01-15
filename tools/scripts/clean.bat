@echo off
setlocal

call "%~dp0setvars.bat" %~dp0..\..

if "%1" == "all" (
    del /s/f/q "%INSTALL_DIR%" > nul && rmdir /s/q "%INSTALL_DIR%" > nul
)

if exist "%BUILD_DIR%" (
    del /s/f/q "%BUILD_DIR%" > nul && rmdir /s/q "%BUILD_DIR%" > nul
)