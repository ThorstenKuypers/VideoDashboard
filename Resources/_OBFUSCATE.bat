
call %1_MERGE.bat %1
C:\Users\thorsten\Downloads\2.0_rc7_bin\Obfuscar.Console.exe %1obfus.prj

copy /V /Y %1obfus\%2 /B %1__deployment\%2
copy /V /Y %1libDataLogging.dll %1__deployment\libDataLogging.dll
copy /V /Y %1libLDF.dll %1__deployment\libLDF.dll
copy /V /Y %1libds.dll %1__deployment\libds.dll
copy /V /Y %1TVOFilter.ax %1__deployment\TVOFilter.ax
copy /V /Y %1DashView.exe %1__deployment\DashView.exe
copy /V /Y %1log4net.dll %1__deployment\log4net.dll
copy /V /Y %1log4net.xml %1__deployment\log4net.xml