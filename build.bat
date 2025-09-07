@echo off
setlocal enabledelayedexpansion

set "DIR_BUILD=build"
set "DIR_BUILD_WEB=build-web"
set "PLATFORM_WEB=false"
set "BUILD_TYPE=Debug"

where cmake >nul 2>nul
if %errorlevel% neq 0 (
    echo CMake is not installed or not in the PATH. Please install CMake and add it to your PATH.
    exit /b 1
)

for %%a in (%*) do (
    if "%%a"=="--web" (
        set "PLATFORM_WEB=true"
    )
)

if "%PLATFORM_WEB%"=="true" (
    echo Building for web...
    if not exist %DIR_BUILD_WEB% (
        mkdir build-web
        emcmake cmake -S . -B %DIR_BUILD_WEB%
    )
    cmake %DIR_BUILD_WEB% -DCMAKE_BUILD_TYPE=%BUILD_TYPE%
    cmake --build %DIR_BUILD_WEB% --verbose 
    @REM cmake --install %DIR_BUILD_WEB%
) else (
    echo Building for desktop...
    if not exist %DIR_BUILD% (
        cmake -S . -B %DIR_BUILD% -G "MinGW Makefiles"
    )
    
    cmake %DIR_BUILD% -DCMAKE_BUILD_TYPE=%BUILD_TYPE%
    cmake --build %DIR_BUILD%
    @REM cmake --install %DIR_BUILD%
)



