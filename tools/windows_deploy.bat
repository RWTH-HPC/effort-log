%QTDIR%\bin\windeployqt %APPVEYOR_BUILD_FOLDER%\effort-log.exe
move %APPVEYOR_BUILD_FOLDER%\effort-log.exe %APPVEYOR_BUILD_FOLDER%\effort-log_v%APPVEYOR_BUILD_VERSION%.exe
