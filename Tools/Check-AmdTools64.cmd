@echo off
title AmdTools64 Driver Check

powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%~dp0Check-AmdTools64.ps1"

if errorlevel 1 (
    echo.
    echo PowerShell script returned an error.
    pause
)