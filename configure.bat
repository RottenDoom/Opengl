@echo off
REM Build Everything

ECHO "Configuring project..."

REM Engine
IF NOT EXIST build (mkdir build)

PUSHD build
cmake -G "MinGW Makefiles" ..
POPD
IF %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && PAUSE)
ECHO "Configured."