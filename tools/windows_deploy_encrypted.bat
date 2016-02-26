%QTDIR%\bin\windeployqt %APPVEYOR_BUILD_FOLDER%\effort-log.exe
copy C:\OpenSSL-Win32\libeay32.dll %APPVEYOR_BUILD_FOLDER%\libeay32.dll
copy C:\OpenSSL-Win32\libssl32.dll %APPVEYOR_BUILD_FOLDER%\libssl32.dll
copy C:\OpenSSL-Win32\ssleay32.dll %APPVEYOR_BUILD_FOLDER%\ssleay32.dll
move %APPVEYOR_BUILD_FOLDER%\effort-log.exe %APPVEYOR_BUILD_FOLDER%\effort-log_v%APPVEYOR_BUILD_VERSION%_encrypted.exe
