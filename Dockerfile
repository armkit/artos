FROM ubuntu:latest

ENV DEBIAN_FRONTEND=noninteractive

COPY scripts/docker-include-sources.sh /
RUN sh /docker-include-sources.sh

COPY scripts/docker-install-deps.sh /
RUN sh /docker-install-deps.sh

ARG RANDOM=0
COPY . /artos

COPY scripts/docker-run-meson.sh /
RUN sh /docker-run-meson.sh
