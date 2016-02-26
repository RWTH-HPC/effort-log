%QTDIR%\bin\windeployqt %APPVEYOR_BUILD_FOLDER%\build\release\effort-log.exe --dir dist
copy C:\OpenSSL-Win32\libeay32.dll %APPVEYOR_BUILD_FOLDER%\build\release\dist\libeay32.dll
copy C:\OpenSSL-Win32\libssl32.dll %APPVEYOR_BUILD_FOLDER%\build\release\dist\libssl32.dll
copy C:\OpenSSL-Win32\ssleay32.dll %APPVEYOR_BUILD_FOLDER%\build\release\dist\ssleay32.dll
7z a %APPVEYOR_BUILD_FOLDER%\effort-log_v%APPVEYOR_BUILD_VERSION%_encrypted.zip %APPVEYOR_BUILD_FOLDER%\build\release\dist\*
