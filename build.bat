@echo off
echo.
docker build --build-arg RANDOM=%RANDOM% -t artos:latest .
docker run -v %CD%:/mnt -t artos:latest cp -r /artos/build /mnt
