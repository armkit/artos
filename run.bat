@echo off
echo.

REM **************************************************************************
REM *
REM *                   ARTOS Operating System.
REM *                 Copyright (C) 2020  ARMKit.
REM *
REM **************************************************************************
REM * @file   build.bat
REM * @brief  ARTOS build instruction, Windows NT.
REM **************************************************************************
REM *
REM * This program is free software; you can redistribute it and/or
REM * modify it under the terms of the GNU General Public License
REM * as published by the Free Software Foundation; either version 2
REM * of the License, or (at your option) any later version.
REM *
REM * This program is distributed in the hope that it will be useful,
REM * but WITHOUT ANY WARRANTY; without even the implied warranty of
REM * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
REM * GNU General Public License for more details.
REM *
REM * You should have received a copy of the GNU General Public License
REM * along with this program; if not, write to the Free Software
REM * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
REM * 02110-1301, USA.
REM *
REM **************************************************************************

if %errorlevel% neq 0 exit /b %errorlevel%

docker run -p 5000:5000 -it artos:latest %*
