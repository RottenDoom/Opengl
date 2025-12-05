@echo off
REM Build Everything

ECHO "Building everything..."

REM Engine
IF NOT EXIST build (mkdir build)

PUSHD build
make -j8
POPD
IF %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && PAUSE)
ECHO "All assemblies built successfully."