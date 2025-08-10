Write-Host "Hello from PowerShell running in Wine!" -ForegroundColor Green
Write-Host "Current directory: $(Get-Location)" -ForegroundColor Yellow
Write-Host "Current date: $(Get-Date)" -ForegroundColor Cyan
Write-Host "Windows version: $([Environment]::OSVersion)" -ForegroundColor Magenta
Write-Host "PowerShell version: $($PSVersionTable.PSVersion)" -ForegroundColor Blue

# Test some basic Windows functionality
$processes = Get-Process | Select-Object -First 5
Write-Host "`nFirst 5 processes:" -ForegroundColor White
$processes | Format-Table Name, Id, CPU -AutoSize

Write-Host "`nTest completed successfully!" -ForegroundColor Green
