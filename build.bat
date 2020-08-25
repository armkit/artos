@echo off
echo.
docker pull armkitdock/artos:latest
docker run -v %cd%:/artos -it armkitdock/artos:latest sh /artos/meson.sh
