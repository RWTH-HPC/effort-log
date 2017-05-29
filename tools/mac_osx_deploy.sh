#!/bin/sh

if [ "$#" -eq 0 ]; then
  echo "Version numbering missing"
else
  if [ "$#" -eq 1 ]; then
    /usr/local/opt/qt5/bin/macdeployqt ${TRAVIS_BUILD_DIR}/build/release/EffortLog.app -always-overwrite -verbose=3 -appstore-compliant -dmg
    mv ${TRAVIS_BUILD_DIR}/build/release/EffortLog.dmg ${TRAVIS_BUILD_DIR}/build/release/EffortLog_$1_osx.dmg
    otool -L ${TRAVIS_BUILD_DIR}/build/release/EffortLog.app/Contents/MacOS/EffortLog
  elif [ $2 = "crypt" ]; then
    install_name_tool -change /usr/local/opt/openssl/lib/libssl.1.0.0.dylib @executable_path/../Frameworks/libssl.1.0.0.dylib
    install_name_tool -change /usr/local/opt/openssl/lib/libcrypto.1.0.0.dylib @executable_path/../Frameworks/libcrypto.1.0.0.dylib
    /usr/local/opt/qt5/bin/macdeployqt ${TRAVIS_BUILD_DIR}/build/release/EffortLog.app -always-overwrite -verbose=3 -appstore-compliant -dmg
    mv ${TRAVIS_BUILD_DIR}/build/release/EffortLog.dmg ${TRAVIS_BUILD_DIR}/build/release/EffortLog_$1_osx_encrypted.dmg
    otool -L ${TRAVIS_BUILD_DIR}/build/release/EffortLog.app/Contents/MacOS/EffortLog
  else
    echo "Unknown parameter < $2 >"
  fi
fi
