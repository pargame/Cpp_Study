@echo off
setlocal enabledelayedexpansion

echo [Setup] Initializing Environment...
call "%~dp0..\setup_env.bat"

if not exist ..\Week25\include\asio.hpp (
    echo [Warning] Asio header not found in Week25. Trying to download here...
    call "%~dp0..\Week25\setup_asio.bat"
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
echo    .\Debug\GameServer.exe
echo.
pause
