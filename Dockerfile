#**************************************************************************
#*
#*                   ARTOS Operating System.
#*                 Copyright (C) 2020  ARMKit.
#*
#**************************************************************************
#* @file   Dockerfile
#* @brief  Docker instructions to build ARTOS using an ubuntu image.
#**************************************************************************
#*
#* This program is free software; you can redistribute it and/or
#* modify it under the terms of the GNU General Public License
#* as published by the Free Software Foundation; either version 2
#* of the License, or (at your option) any later version.
#*
#* This program is distributed in the hope that it will be useful,
#* but WITHOUT ANY WARRANTY; without even the implied warranty of
#* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#* GNU General Public License for more details.
#*
#* You should have received a copy of the GNU General Public License
#* along with this program; if not, write to the Free Software
#* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
#* 02110-1301, USA.
#*
#**************************************************************************

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
