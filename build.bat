@echo off

if not exist build mkdir build
pushd build
cl -Zi ..\src\main.c ..\src\set.c ..\src\list.c ..\src\logging.c
popd