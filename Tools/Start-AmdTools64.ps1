$ServiceName = "AmdTools64"

$BasePath = Split-Path $PSScriptRoot -Parent
$DriverPath = Join-Path $BasePath "Driver\AmdTools64.sys"

if (-not (Test-Path $DriverPath)) {
    Write-Host "[ERROR] Driver not found:" -ForegroundColor Red
    Write-Host $DriverPath
    Read-Host "Press Enter to exit"
    exit 1
}

$DriverPath = (Resolve-Path $DriverPath).Path

Write-Host "Driver path: $DriverPath" -ForegroundColor Cyan

$existing = Get-CimInstance Win32_SystemDriver -Filter "Name='$ServiceName'" -ErrorAction SilentlyContinue

if ($existing) {
    if ($existing.State -eq "Running") {
        Write-Host "[OK] $ServiceName is already running." -ForegroundColor Green
        Write-Host "Registered path: $($existing.PathName)"
        Read-Host "Press Enter to exit"
        exit 0
    }

    & sc.exe config $ServiceName type= kernel start= demand binPath= "`"$DriverPath`""
}
else {
    & sc.exe create $ServiceName type= kernel start= demand binPath= "`"$DriverPath`""
}

if ($LASTEXITCODE -ne 0) {
    Write-Host "[ERROR] Could not configure driver service." -ForegroundColor Red
    Read-Host "Press Enter to exit"
    exit 1
}

Write-Host ""
Write-Host "Starting $ServiceName..." -ForegroundColor Cyan

& sc.exe start $ServiceName

Start-Sleep -Seconds 1

$driver = Get-CimInstance Win32_SystemDriver -Filter "Name='$ServiceName'" -ErrorAction SilentlyContinue

if ($driver -and $driver.State -eq "Running") {
    Write-Host "[LOADED] $ServiceName is running." -ForegroundColor Green
    Write-Host "Path: $($driver.PathName)"
}
else {
    Write-Host "[FAILED] Driver did not enter Running state." -ForegroundColor Red
}
