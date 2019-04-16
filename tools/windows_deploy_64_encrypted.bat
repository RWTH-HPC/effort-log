mkdir %APPVEYOR_BUILD_FOLDER%\build\release\dist
move %APPVEYOR_BUILD_FOLDER%\build\release\effort-log.exe %APPVEYOR_BUILD_FOLDER%\build\release\dist\effort-log.exe
%QTDIR%\bin\windeployqt %APPVEYOR_BUILD_FOLDER%\build\release\dist\effort-log.exe
copy C:\OpenSSL-Win62\libeay64.dll %APPVEYOR_BUILD_FOLDER%\build\release\dist\libeay64.dll
copy C:\OpenSSL-Win62\libssl64.dll %APPVEYOR_BUILD_FOLDER%\build\release\dist\libssl64.dll
copy C:\OpenSSL-Win62\ssleay64.dll %APPVEYOR_BUILD_FOLDER%\build\release\dist\ssleay64.dll
7z a %APPVEYOR_BUILD_FOLDER%\effort-log_%APPVEYOR_REPO_TAG_NAME%_win64_encrypted.zip %APPVEYOR_BUILD_FOLDER%\build\release\dist\*
