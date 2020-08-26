FROM ubuntu:latest

COPY docker-apt-sources.list /etc/apt/sources.list

COPY docker-install-deps.sh /

RUN sh /docker-install-deps.sh

COPY . /artos

RUN sh /artos/docker-run-meson.sh
