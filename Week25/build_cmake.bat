@echo off
setlocal enabledelayedexpansion

echo [Setup] Initializing Environment...
call "%~dp0..\setup_env.bat"

if not exist include (
    echo [Info] Running setup_asio.bat first...
    call "%~dp0setup_asio.bat"
)

if not exist build mkdir build
cd build

echo.
echo [CMake] Configuring...
cmake ..
if %errorlevel% neq 0 (
    echo [ERROR] CMake configuration failed.
    pause
    exit /b
)

echo.
echo [CMake] Building...
cmake --build .
if %errorlevel% neq 0 (
    echo [ERROR] Build failed.
    pause
    exit /b
)

echo.
echo [Success] Build Complete!
echo.
echo === How to Run ===
echo    .\Debug\01_TimerSync.exe
echo    .\Debug\02_TimerAsync.exe
echo.
pause
