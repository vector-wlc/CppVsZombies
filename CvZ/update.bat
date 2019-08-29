
@echo off

%~d0 && cd %~dp0

set SRC_PATH=https://pvz.lmintlcx.com/cvz/
set DST_PATH=%~dp0
set FILES=pvz_h/include/libpvz.h pvz_h/include/pvz_data.h pvz_h/include/pvz.h pvz_h/lib/libpvz.a

for %%f in (%FILES%) do (
  if not exist "%%~dpf" md "%%~dpf"
  bitsadmin.exe /transfer "test" "%SRC_PATH%/%%f" "%DST_PATH%\%%f"
)

rem pause
