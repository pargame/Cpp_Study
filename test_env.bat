@echo off
call setup_env.bat
echo.
echo [DEBUG] Checking PATH...
echo %PATH% | findstr /I "CMake"
if %errorlevel% equ 0 (
    echo [DEBUG] CMake path found in PATH variable.
) else (
    echo [DEBUG] CMake path NOT found in PATH variable.
)

echo.
echo [DEBUG] Trying to run cmake...
cmake --version
if %errorlevel% neq 0 (
    echo [FAIL] cmake command failed.
) else (
    echo [SUCCESS] cmake command works.
)
