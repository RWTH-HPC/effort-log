mkdir %APPVEYOR_BUILD_FOLDER%\build\release\dist
move %APPVEYOR_BUILD_FOLDER%\build\release\effort-log.exe %APPVEYOR_BUILD_FOLDER%\build\release\dist\effort-log.exe
%QTDIR%\bin\windeployqt %APPVEYOR_BUILD_FOLDER%\build\release\dist\effort-log.exe
7z a %APPVEYOR_BUILD_FOLDER%\effort-log_%APPVEYOR_REPO_TAG_NAME%_win64.zip %APPVEYOR_BUILD_FOLDER%\build\release\dist\*
