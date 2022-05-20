@echo OFF

set /A counter = 1

:loop
CALL :ping 8.8.8.8
SET PING1=%ms%
ECHO Latency: %ping1% ms
IF /I %ping1% gtr 500 (GOTO trace)
timeout 1 /nobreak > nul
GOTO loop

:ping
SET ms=Error
FOR /F "tokens=4 delims==ms" %%i IN ('ping.exe -n 1 %1 ^| FIND "ms"') DO SET ms=%%i
exit/b

:trace
echo Trace %counter% at %time& >> tracert.txt
timeout 1 /nobreak > nul
tracert 8.8.8.8 >> tracert.txt
timeout 1 /nobreak > nul
echo. >> tracert.txt
timeout 1 /nobreak > nul
echo. >> tracert.txt
timeout 1 /nobreak > nul
echo Event logged
SET /A counter = %counter% + 1
if %counter% gtr 10 (GOTO:EOF)
GOTO loop
