@echo off
echo ========================================
echo    Wine Test Results
echo ========================================
echo.

echo 1. Basic Information:
echo    Current directory: %CD%
echo    Date: %DATE%
echo    Time: %TIME%
echo    User: %USERNAME%
echo    Computer: %COMPUTERNAME%
echo.

echo 2. Environment Variables:
echo    PATH: %PATH%
echo    TEMP: %TEMP%
echo    WINDIR: %WINDIR%
echo.

echo 3. File System Test:
if exist "C:\" (
    echo    C:\ drive exists
) else (
    echo    C:\ drive not accessible
)

if exist "Z:\" (
    echo    Z:\ drive exists (Docker mount)
) else (
    echo    Z:\ drive not accessible
)
echo.

echo 4. Registry Test:
reg query "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion" /v ProductName >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    echo    Registry access: OK
) else (
    echo    Registry access: Failed
)
echo.

echo 5. Wine Version:
wine --version
echo.

echo ========================================
echo    Test completed!
echo ========================================
pause
