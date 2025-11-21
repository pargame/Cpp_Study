@echo off
setlocal enabledelayedexpansion

echo [Setup] Checking environment...

:: Check if cl is already available
where cl >nul 2>nul
if %errorlevel% neq 0 (
    echo 'cl' not found. Attempting to initialize Visual Studio environment...
    
    :: Try to find VsDevCmd.bat in the known path
    set "VS_DEV_CMD=C:\Program Files\Microsoft Visual Studio\18\Community\Common7\Tools\VsDevCmd.bat"
    
    if exist "!VS_DEV_CMD!" (
        echo Found VsDevCmd.bat. Initializing...
        call "!VS_DEV_CMD!" -arch=x64 -host_arch=x64
    ) else (
        echo [ERROR] Could not find VsDevCmd.bat at:
        echo !VS_DEV_CMD!
        echo Please check your Visual Studio installation.
        pause
        exit /b
    )
)

:: Check cl again
where cl >nul 2>nul
if %errorlevel% neq 0 (
    echo [ERROR] Still cannot find 'cl' after initialization.
    pause
    exit /b
)

echo [OK] Compiler found.
cl /Bv

echo.
echo [Build] Compiling main.cpp...
:: /std:c++latest for C++23 support, /EHsc for exception handling
cl /EHsc /std:c++latest main.cpp
if %errorlevel% neq 0 (
    echo [ERROR] Compilation failed.
    pause
    exit /b
)

echo.
echo [Run] Running HelloThread.exe...
HelloThread.exe
echo.
echo [Success] Environment is ready!
pause
