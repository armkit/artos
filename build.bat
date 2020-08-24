@echo off
docker pull armkitdock/artos:latest
docker run -v %cd%:/shared -it armkitdock/artos:latest sh shared/meson.sh
