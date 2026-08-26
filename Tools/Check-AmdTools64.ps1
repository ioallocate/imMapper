$DriverName = "AmdTools64.sys"

Write-Host "Checking for $DriverName..." -ForegroundColor Cyan

$driver = Get-CimInstance Win32_SystemDriver |
    Where-Object {
        [System.IO.Path]::GetFileName($_.PathName.Trim('"')) -ieq $DriverName
    }

if ($driver) {
    Write-Host ""
    Write-Host "[LOADED] $DriverName was found." -ForegroundColor Green
    Write-Host "Service Name : $($driver.Name)"
    Write-Host "State        : $($driver.State)"
    Write-Host "Status       : $($driver.Status)"
    Write-Host "Start Mode   : $($driver.StartMode)"
    Write-Host "Path         : $($driver.PathName)"

    if ($driver.State -eq "Running") {
        Write-Host ""
        Write-Host "$DriverName is currently running." -ForegroundColor Green
    }
    else {
        Write-Host ""
        Write-Host "$DriverName exists, but is not currently running." -ForegroundColor Yellow
    }
}
else {
    Write-Host ""
    Write-Host "[NOT LOADED] $DriverName was not found in the system driver list." -ForegroundColor Red
}

Write-Host ""