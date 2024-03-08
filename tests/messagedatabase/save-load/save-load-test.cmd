:: This file is part of the test suite for Private Message Database.
:: Copyright (C) 2015, 2024  Dirk Stolle
::
:: This program is free software: you can redistribute it and/or modify
:: it under the terms of the GNU General Public License as published by
:: the Free Software Foundation, either version 3 of the License, or
:: (at your option) any later version.
::
:: This program is distributed in the hope that it will be useful,
:: but WITHOUT ANY WARRANTY; without even the implied warranty of
:: MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
:: GNU General Public License for more details.
::
:: You should have received a copy of the GNU General Public License
:: along with this program.  If not, see <http://www.gnu.org/licenses/>.

@echo off

:: check parameter
if "%1" EQU "" (
  echo Error: Expecting one parameter - executable for this test!
  exit /B 1
)

:: create base directory where the test shall take place
: tmp-dir
SET SAVE_DIR=%TEMP%\test-mdb-save-load-%RANDOM%
if EXISTS "%SAVE_DIR%" GOTO tmp-dir
md "%SAVE_DIR%"

# run test
"$1" "$SAVE_DIR"
# ...and save its exit code
SET TEST_EXIT_CODE_FIRST=%ERRORLEVEL%

# run test with slash at end of path
"$1" $SAVE_DIR\
# ...and save its exit code
SET TEST_EXIT_CODE_SECOND=%ERRORLEVEL%

TOTAL=0

if %TEST_EXIT_CODE_FIRST% NEQ 0 (
  echo ERROR: The first part of the MessageDatabase save/load test, path without slash, failed!
  SET TOTAL=1
)

if %TEST_EXIT_CODE_SECOND% NEQ 0 (
  echo ERROR: The second part of the MessageDatabase save/load test, path with slash, failed!
  SET TOTAL=2
)

:: clean up test directory
rd /S /Q "%SAVE_DIR%"

if %TOTAL% EQU 0 (
  exit /B 0
) ELSE (
  exit /B 1
)
