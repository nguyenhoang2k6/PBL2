@echo off
title Run C++ Project
echo ============================
echo   COMPILING PROJECT...
echo ============================

g++ -std=c++11 -Wall -O2 -o main.exe *.cpp

if %errorlevel% neq 0 (
    echo.
    echo ❌ Compilation failed!
    pause
    exit /b
)

echo.
echo ============================
echo      RUNNING PROGRAM
echo ============================
echo.

main.exe

echo.
pause
