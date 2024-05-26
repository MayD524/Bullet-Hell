@echo off

gcc -Os -g -o ./build/bullet_hell.exe ./src/*.c -I./raylib/include -I./include ./build/raylib.dll ./build/lua54.dll 

IF NOT EXIST ".\build\scripts" MKDIR .\build\scripts
COPY .\src\lua\*.lua .\build\scripts