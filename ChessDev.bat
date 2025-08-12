@echo off
setlocal enabledelayedexpansion

:: =============================================================================
:: Detects and runs Chess.exe with 'dev' argument
:: =============================================================================

title Chess Development Runner
color 0A

echo.
echo ========================================
echo  Chess Development Environment
echo ========================================
echo.

:: Change to script directory to ensure relative paths work
cd /d "%~dp0"

:: Initialize variables
set "CHESS_EXE="
set "BUILD_TYPE="

:: Search order for Chess.exe (most common locations)
set "SEARCH_PATHS=build\bin build\Release build\Debug bin Release Debug out\build\x64-Release out\build\x64-Debug build\x64\Release build\x64\Debug cmake-build-release\bin cmake-build-debug\bin"

:: Search for Chess.exe in common build directories
echo Searching for Chess.exe...
for %%P in (%SEARCH_PATHS%) do (
    if exist "%%P\Chess.exe" (
        set "CHESS_EXE=%%P\Chess.exe"
        set "BUILD_TYPE=%%P"
        echo [FOUND] Chess.exe located at: %%P\Chess.exe
        goto :found_exe
    )
)

:: If not found in subdirectories, check current directory
if exist "Chess.exe" (
    set "CHESS_EXE=Chess.exe"
    set "BUILD_TYPE=current directory"
    echo [FOUND] Chess.exe located in current directory
    goto :found_exe
)

:: Chess.exe not found - provide helpful error message
echo [ERROR] Chess.exe not found!
echo.
echo The executable was not found in any of the following locations:
for %%P in (%SEARCH_PATHS%) do (
    echo   - %%P\Chess.exe
)
echo   - Chess.exe (current directory)
echo.
echo Please ensure the project has been built successfully.
echo.
echo Common build commands:
echo   CMake + MinGW:     cmake .. -G "MinGW Makefiles" ^&^& cmake --build .
echo   CMake + MSVC:      cmake .. ^&^& cmake --build . --config Release
echo   Visual Studio:     Open .sln file and build solution
echo.
goto :error_exit

:found_exe
:: Display system information
echo.
echo Build Information:
echo   Location: %BUILD_TYPE%
echo   Full Path: %CD%\%CHESS_EXE%

:: Check if file is actually executable
if not exist "%CHESS_EXE%" (
    echo [ERROR] File exists but cannot be accessed: %CHESS_EXE%
    goto :error_exit
)

:: Get file size and modification date for verification
for %%F in ("%CHESS_EXE%") do (
    echo   Size: %%~zF bytes
    echo   Modified: %%~tF
)

echo.
echo ========================================
echo  Starting Chess in Development Mode
echo ========================================
echo.
echo Executing: %CHESS_EXE% dev
echo.

:: Run the executable with dev argument
"%CHESS_EXE%" dev

:: Capture exit code
set "EXIT_CODE=!errorlevel!"

echo.
echo ========================================
echo  Execution Complete
echo ========================================

:: Display exit status
if !EXIT_CODE! equ 0 (
    echo [SUCCESS] Chess exited normally (code: !EXIT_CODE!)
    color 0A
) else (
    echo [WARNING] Chess exited with code: !EXIT_CODE!
    color 0E
)

echo.
echo Press any key to exit...
pause >nul
exit /b !EXIT_CODE!

:error_exit
color 0C
echo.
echo Press any key to exit...
pause >nul
exit /b 1

:: =============================================================================
:: End of Script
:: =============================================================================