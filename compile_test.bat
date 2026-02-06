@echo off
REM filepath: g:\Mi unidad\Repositorios\proyecto_batalla_naval\codigo_fuente\compile_tests.bat

echo ========================================
echo  COMPILANDO PRUEBAS UNITARIAS
echo ========================================

set SRC_DIR=src
set INCLUDE_DIR=include
set TESTS_DIR=tests
set BIN_DIR=%USERPROFILE%\Desktop

REM Los ejecutables se generan en el escritorio para evitar problemas con Google Drive

REM ===== TEST 1: DECK Y CARD =====
echo.
echo [1/3] Compilando test_deck.exe...

gcc -Wall -Wextra -I%INCLUDE_DIR% ^
    %TESTS_DIR%\test_deck.c ^
    %SRC_DIR%\model\bs_Card.c ^
    %SRC_DIR%\model\bs_Deck.c ^
    %SRC_DIR%\utils\bs_error_handler.c ^
    -o %BIN_DIR%\test_deck.exe

if %ERRORLEVEL% EQU 0 (
    echo [OK] test_deck.exe compilado correctamente
) else (
    echo [ERROR] Fallo en compilacion de test_deck
    exit /b 1
)

REM ===== TEST 2: FLEET Y SHIP =====
echo.
echo [2/3] Compilando test_fleet.exe...

gcc -Wall -Wextra -I%INCLUDE_DIR% ^
    %TESTS_DIR%\test_fleet.c ^
    %SRC_DIR%\model\bs_Ship.c ^
    %SRC_DIR%\model\bs_Fleet.c ^
    %SRC_DIR%\utils\bs_error_handler.c ^
    -o %BIN_DIR%\test_fleet.exe

if %ERRORLEVEL% EQU 0 (
    echo [OK] test_fleet.exe compilado correctamente
) else (
    echo [ERROR] Fallo en compilacion de test_fleet
    exit /b 1
)

REM ===== TEST 3: ERROR HANDLER =====
echo.
echo [3/3] Compilando test_error_handler.exe...

gcc -Wall -Wextra -I%INCLUDE_DIR% ^
    %TESTS_DIR%\test_error_handler.c ^
    %SRC_DIR%\utils\bs_error_handler.c ^
    -o %BIN_DIR%\test_error_handler.exe

if %ERRORLEVEL% EQU 0 (
    echo [OK] test_error_handler.exe compilado correctamente
) else (
    echo [ERROR] Fallo en compilacion de test_error_handler
    exit /b 1
)

echo.
echo ========================================
echo  COMPILACION COMPLETADA
echo ========================================
echo.
echo Ejecutables generados en: %BIN_DIR%\
echo   - test_deck.exe
echo   - test_fleet.exe
echo   - test_error_handler.exe
echo.
echo Para ejecutar:
echo   cd %BIN_DIR%
echo   .\test_deck.exe
echo   .\test_fleet.exe
echo   .\test_error_handler.exe
echo.

pause