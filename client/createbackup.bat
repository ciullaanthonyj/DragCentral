del c:\dc-prev.rar
ren c:\dc.rar dc-prev.rar
pause
xcopy /e c:\ap\gm\*.* c:\dc\php-server /y
"c:\program files\winrar\rar.exe" a -s -x*.tds -x*.obj -x*.~* c:\dc.rar c:\dc
copy c:\dc.rar d:               