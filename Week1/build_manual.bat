@echo off
setlocal enabledelayedexpansion

echo [Setup] Initializing Visual Studio Environment...
call "C:\Program Files\Microsoft Visual Studio\18\Community\Common7\Tools\VsDevCmd.bat" -arch=x64 -host_arch=x64

if not exist build mkdir build

echo.
echo [1/3] Compiling 01_SmartPointers...
cl /EHsc /std:c++latest /utf-8 /Fe:build/01_SmartPointers.exe src/01_smart_pointers.cpp
if %errorlevel% neq 0 (
    echo [FAIL] 01_SmartPointers
) else (
    echo [OK] 01_SmartPointers
)

echo.
echo [2/3] Compiling 02_Lambdas...
cl /EHsc /std:c++latest /utf-8 /Fe:build/02_Lambdas.exe src/02_lambdas.cpp
if %errorlevel% neq 0 (
    echo [FAIL] 02_Lambdas
) else (
    echo [OK] 02_Lambdas
)

echo.
echo [3/3] Compiling 03_Jthread...
cl /EHsc /std:c++latest /utf-8 /Fe:build/03_Jthread.exe src/03_jthread.cpp

if %errorlevel% neq 0 (
    echo [FAIL] 03_Jthread
) else (
    echo [OK] 03_Jthread
)

echo.
echo [Success] Build finished. Executables are in 'Week1/build' folder.

