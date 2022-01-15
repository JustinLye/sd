@echo off
setlocal
call "%~dp0setvars.bat" %~dp0..\..
call "%~dp0vcvars.bat"
cmake -S "%SOURCE_DIR%" -B "%BUILD_DIR%" -G "NMake Makefiles" --install-prefix "%INSTALL_DIR%"
pushd "%BUILD_DIR%"
nmake
popd
cmake --install "%BUILD_DIR%"