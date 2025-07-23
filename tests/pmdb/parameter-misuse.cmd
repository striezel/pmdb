:: Script to test wrong values of parameters.
::
::  Copyright (C) 2025  Dirk Stolle
::
::  This program is free software: you can redistribute it and/or modify
::  it under the terms of the GNU General Public License as published by
::  the Free Software Foundation, either version 3 of the License, or
::  (at your option) any later version.
::
::  This program is distributed in the hope that it will be useful,
::  but WITHOUT ANY WARRANTY; without even the implied warranty of
::  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
::  GNU General Public License for more details.
::
::  You should have received a copy of the GNU General Public License
::  along with this program.  If not, see <http://www.gnu.org/licenses/>.

@echo off

:: 1st parameter = executable path
if "%1" EQU "" (
  echo First parameter must be executable file!
  exit /B 1
)
SET EXECUTABLE=%1

:: 2nd parameter = path to test-messages.xml
if "%2" EQU "" (
  echo Second parameter must be XML file!
  exit /B 1
)
SET XML_FILE=%2

:: --xml: file does not exist
"%EXECUTABLE%" --no-save --no-load-default --xml does-not-exist.xml
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 for non-existent XML file.
  exit /B 1
)

:: --xml: missing file name
"%EXECUTABLE%" --no-save --no-load-default --xml
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when XML file name was missing.
  exit /B 1
)

:: --xml: same file given twice
"%EXECUTABLE%" --no-save --no-load-default --xml "%XML_FILE%" --xml "%XML_FILE%"
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when same XML file was given twice.
  exit /B 1
)

:: --xml=file.xml: file does not exist
"%EXECUTABLE%" --no-save --no-load-default --xml=does-not-exist.xml
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 for non-existent XML file.
  exit /B 1
)

:: --xml=file.xml: same file given twice
"%EXECUTABLE%" --no-save --no-load-default "--xml=%XML_FILE%" "--xml=%XML_FILE%"
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when same XML file was given twice.
  exit /B 1
)

:: --save: parameter given twice
"%EXECUTABLE%" --no-load-default --xml "%XML_FILE%" --save --save
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when save option was given twice.
  exit /B 1
)

:: --save and --no-save at the same time: conflict
"%EXECUTABLE%" --no-load-default --xml "%XML_FILE%" --save --no-save
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when save and no-save option was given.
  exit /B 1
)

:: --no-save: parameter given twice
"%EXECUTABLE%" --no-load-default --xml "%XML_FILE%" --no-save --no-save
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when no-save option was given twice.
  exit /B 1
)

:: --html: parameter given twice
"%EXECUTABLE%" --no-save --no-load-default --xml "%XML_FILE%" --html --html
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when html option was given twice.
  exit /B 1
)

:: --xhtml: parameter given twice
"%EXECUTABLE%" --no-save --no-load-default --xml "%XML_FILE%" --xhtml --xhtml
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when xhtml option was given twice.
  exit /B 1
)

:: --html and --xhtml at the same time: conflict
"%EXECUTABLE%" --no-save --no-load-default --xml "%XML_FILE%" --html --xhtml
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when html and xhtml options were given.
  exit /B 1
)

:: --load-default: parameter given twice
"%EXECUTABLE%" --no-save --xml "%XML_FILE%" --load-default --load-default
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when load-default option was given twice.
  exit /B 1
)

:: --no-load-default: parameter given twice
"%EXECUTABLE%" --no-save --xml "%XML_FILE%" --no-load-default --no-load-default
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when no-load-default option was given twice.
  exit /B 1
)

:: --no-load-default and --load-default: conflict
"%EXECUTABLE%" --no-save --xml "%XML_FILE%" --no-load-default --load-default
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when no-load-default and load-default were given.
  exit /B 1
)

:: --no-br: parameter given twice
"%EXECUTABLE%" --no-save --no-load-default --xml "%XML_FILE%" --no-br --no-br
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when no-br option was given twice.
  exit /B 1
)

:: --no-list: parameter given twice
"%EXECUTABLE%" --no-save --no-load-default --xml "%XML_FILE%" --no-list --no-list
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when no-list option was given twice.
  exit /B 1
)

:: --compress: parameter given twice
"%EXECUTABLE%" --no-save --no-load-default --xml "%XML_FILE%" --compress --compress
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when compress option was given twice.
  exit /B 1
)

:: --no-save-check: parameter given twice
"%EXECUTABLE%" --no-save --no-load-default --xml "%XML_FILE%" --no-save-check --no-save-check
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when no-save-check option was given twice.
  exit /B 1
)

:: --subset-check: parameter given twice
"%EXECUTABLE%" --no-save --no-load-default --xml "%XML_FILE%" --subset-check --subset-check
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when subset-check option was given twice.
  exit /B 1
)

:: --list-from: no name given
"%EXECUTABLE%" --no-save --no-load-default --xml "%XML_FILE%" --list-from
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when list-from option was missing the name.
  exit /B 1
)

:: --list-to: no name given
"%EXECUTABLE%" --no-save --no-load-default --xml "%XML_FILE%" --list-to
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when list-to option was missing the name.
  exit /B 1
)

:: unrecognized parameter given
"%EXECUTABLE%" --no-save --no-load-default --xml "%XML_FILE%" --invalid-param
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when an invalid parameter was given.
  exit /B 1
)

echo Parameter tests succeeded.

exit /B 0
