@echo off
echo [Setup] Downloading Standalone Asio...

if exist include\asio.hpp (
    echo [Info] Asio already exists.
    exit /b
)

mkdir include
powershell -Command "Invoke-WebRequest -Uri 'https://github.com/chriskohlhoff/asio/archive/refs/tags/asio-1-30-2.zip' -OutFile 'asio.zip'"
powershell -Command "Expand-Archive -Path 'asio.zip' -DestinationPath '.'"
move asio-asio-1-30-2\asio\include\* include\
rmdir /s /q asio-asio-1-30-2
del asio.zip

echo [Success] Asio downloaded to ./include
pause
