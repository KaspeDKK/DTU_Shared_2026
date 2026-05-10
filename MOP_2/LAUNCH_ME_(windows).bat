@echo off

echo Starting server...
start "" "%~dp0cmake-build-default-mingw\server.exe"

timeout /t 1 >nul

echo Starting Python GUI...
cd /d "%~dp0python_gui"
python main.py

pause