FROM ubuntu:latest

ENV DEBIAN_FRONTEND=noninteractive

COPY docker-include-sources.sh /
RUN sh /docker-include-sources.sh

COPY docker-install-deps.sh /
RUN sh /docker-install-deps.sh

ARG RANDOM=0
COPY . /artos

COPY docker-run-meson.sh /
RUN sh /docker-run-meson.sh
