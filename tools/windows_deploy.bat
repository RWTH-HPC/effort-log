%QTDIR%\bin\windeployqt %APPVEYOR_BUILD_FOLDER%\build\release\effort-log.exe --dir dist
7z a %APPVEYOR_BUILD_FOLDER%\effort-log_v%APPVEYOR_BUILD_VERSION%.zip %APPVEYOR_BUILD_FOLDER%\build\release\dist\*
