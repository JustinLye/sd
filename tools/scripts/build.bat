@echo off
setlocal
call "%~dp0setvars.bat" %~dp0..\..
call "%~dp0vcvars.bat"
cmake -S "%SOURCE_DIR%" -B "%BUILD_DIR%" -G "Visual Studio 16 2019" --install-prefix "%INSTALL_DIR%"
msbuild "%BUILD_DIR%"
cmake --install "%BUILD_DIR%"