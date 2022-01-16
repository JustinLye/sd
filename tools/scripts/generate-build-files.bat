@echo off
setlocal
cmake -S "%~dp0..\.." -B "%~dp0\..\..\build" -G "Visual Studio 16 2019" --install-prefix "%~dp0\..\..\install"