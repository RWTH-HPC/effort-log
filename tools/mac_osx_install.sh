#!/usr/bin/env bash

cd /tmp
QT_BASE_DIR=/opt/qt57
sudo mkdir -p "$QT_BASE_DIR"
wget https://download.qt.io/official_releases/qt/5.7/5.7.0/single/qt-everywhere-opensource-src-5.7.0.tar.gz
gunzip qt-everywhere-opensource-src-5.7.0.tar.gz
tar xf qt-everywhere-opensource-src-5.7.0.tar
cd qt-everywhere-opensource-src-5.7.0
./configure \
  -opensource \
  -confirm-license \
  -release \
  -prefix "/opt/qt57" \
  -nomake examples \
  -nomake tests \
  -no-dbus \
  -skip qtpurchasing \
  -skip qtscxml \
  -skip qtsensors \
  -skip qtserialbus \
  -skip qtserialport \
  -skip qtvirtualkeyboard \
  -skip qtwayland \
  -skip qtwinextras \
  -skip qtx11extras

make -j2
sudo make install

export PATH=$QT_BASE_DIR/bin:$PATH
