@echo off
echo.
docker build --build-arg RANDOM=%RANDOM% -t artos:latest .
