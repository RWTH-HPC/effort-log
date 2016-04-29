#!/bin/sh

if [ "$#" -eq 0 ]; then
  echo "Version numbering missing"
else
  if [ "$#" -eq 1 ]; then
    /usr/local/opt/qt5/bin/macdeployqt ${TRAVIS_BUILD_DIR}/build/release/EffortLog.app -always-overwrite -verbose=2 -dmg && mv ${TRAVIS_BUILD_DIR}/build/release/EffortLog.dmg ${TRAVIS_BUILD_DIR}/build/release/EffortLog_$1.dmg
  elif [ $2 = "crypt" ]; then
    /usr/local/opt/qt5/bin/macdeployqt ${TRAVIS_BUILD_DIR}/build/release/EffortLog.app -always-overwrite -verbose=2 -dmg && mv ${TRAVIS_BUILD_DIR}/build/release/EffortLog.dmg ${TRAVIS_BUILD_DIR}/build/release/EffortLog_$1_encrypted.dmg
  else
    echo "Unknown parameter < $2 >"
  fi
fi
