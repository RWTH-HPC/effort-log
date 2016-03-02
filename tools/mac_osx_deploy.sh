#!/bin/sh

if [ "$#" -eq 0 ]; then
  echo "Version numbering missing"
else
  APP="EffortLog"
  if [ "$#" -eq 1 ]; then
    /usr/local/opt/qt5/bin/macdeployqt ${APP}.app -always-overwrite -verbose=2 -qmldir=qml -dmg && mv ${APP}.dmg ${APP}_$1.dmg
  elif [ $2 = "CRYPT" ]; then
    /usr/local/opt/qt5/bin/macdeployqt ${APP}.app -always-overwrite -verbose=2 -qmldir=qml -dmg && mv ${APP}.dmg ${APP}_$1_encrypted.dmg
  else
    echo "Unknown parameter < $2 >"
  fi
fi
