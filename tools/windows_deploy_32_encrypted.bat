mkdir %APPVEYOR_BUILD_FOLDER%\build\release\dist
move %APPVEYOR_BUILD_FOLDER%\build\release\effort-log.exe %APPVEYOR_BUILD_FOLDER%\build\release\dist\effort-log.exe
%QTDIR%\bin\windeployqt %APPVEYOR_BUILD_FOLDER%\build\release\dist\effort-log.exe
copy C:\OpenSSL-Win32\libeay32.dll %APPVEYOR_BUILD_FOLDER%\build\release\dist\libeay32.dll
copy C:\OpenSSL-Win32\libssl32.dll %APPVEYOR_BUILD_FOLDER%\build\release\dist\libssl32.dll
copy C:\OpenSSL-Win32\ssleay32.dll %APPVEYOR_BUILD_FOLDER%\build\release\dist\ssleay32.dll
7z a %APPVEYOR_BUILD_FOLDER%\effort-log_%APPVEYOR_REPO_TAG_NAME%_win32_encrypted.zip %APPVEYOR_BUILD_FOLDER%\build\release\dist\*
