#------------------------------------------------------------------------------#
#  Copyright (c) 2015-2017 by IT Center, RWTH Aachen University
#
#  This file is part of EffortLog, a tool for collecting software
#  development effort.
#
#  EffortLog is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  EffortLog is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with EffortLog.  If not, see <http://www.gnu.org/licenses/>.
#------------------------------------------------------------------------------#

TARGET = effort-log
macx:TARGET = EffortLog
TEMPLATE = app
VERSION = 0.8.1
DEFINES += APP_VERSION=\\\"$$VERSION\\\"
QT += core gui

isEqual(QT_MAJOR_VERSION, 5) {
  QT += widgets
} else {
  error("Qt5 required!")
}

# Compiler flags
CONFIG += c++11
CONFIG(debug, debug|release) {
  CONFIG += warn_on
  win32-msvc*:QMAKE_CXXFLAGS += /WX
  else:       QMAKE_CXXFLAGS += -Werror -Wextra -Wpedantic
}

# Build directory
CONFIG(debug, debug|release) {
  DESTDIR = build/debug
}
CONFIG(release, debug|release) {
  DESTDIR = build/release
}
OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.u
QMAKE_DISTCLEAN += -rf build

# OS X
macx {
  HEADERS += src/appnap.h
  OBJECTIVE_SOURCES += src/appnap.mm
  LIBS += -framework Foundation -framework ApplicationServices

  CONFIG += app_bundle
  QMAKE_INFO_PLIST = $$PWD/resources/Info.plist
  INFO_PLIST_PATH = $$shell_quote($${DESTDIR}/$${TARGET}.app/Contents/Info.plist)
  QMAKE_POST_LINK += /usr/libexec/PlistBuddy -c \"Set :CFBundleGetInfoString Version $${VERSION} IT Center RWTH Achen University\" $${INFO_PLIST_PATH};
  QMAKE_POST_LINK += /usr/libexec/PlistBuddy -c \"Set :CFBundleLongVersionString Version $${VERSION} IT Center RWTH Achen University\" $${INFO_PLIST_PATH};
  QMAKE_POST_LINK +=  /usr/libexec/PlistBuddy -c \"Set :CFBundleShortVersionString $${VERSION}\" $${INFO_PLIST_PATH};
  QMAKE_POST_LINK += /usr/libexec/PlistBuddy -c \"Set :CFBundleVersion $${VERSION}\" $${INFO_PLIST_PATH};
}

# Linux
unix:!macx {
  CONFIG += static
}

# Support for encryption
CONFIG(crypt) {
  message(Configuring EffortLog to be build with encryption.)
  DEFINES += CRYPT
  SOURCES += src/crypt.cc
  SOURCES += src/passworddialog.cc
  HEADERS += src/crypt.h
  HEADERS += src/passworddialog.h
  unix {
    # Check for SLL libraries; if SLL is not found add your local path
    exists(/usr/local/include):INCLUDEPATH += /usr/local/include
    exists(/usr/local/ssl/include):INCLUDEPATH += /usr/local/ssl/include
    exists(/usr/local/openssl/include):INCLUDEPATH += /usr/local/openssl/include
    exists(/usr/local/opt/openssl/include):INCLUDEPATH += /usr/local/opt/openssl/include
    exists(/opt/ssl/include):INCLUDEPATH += /opt/ssl/include
    exists(/opt/openssl/include):INCLUDEPATH += /opt/openssl/include
    exists(/usr/local/lib):LIBS += -L/usr/local/lib
    exists(/usr/local/ssl/lib):LIBS += -L/usr/local/ssl/lib
    exists(/usr/local/openssl/lib):LIBS += -L/usr/local/openssl/lib
    exists(/usr/local/opt/openssl/lib):LIBS += -L/usr/local/opt/openssl/lib
    exists(/opt/ssl/lib):LIBS += -L/opt/ssl/lib
    exists(/opt/openssl/lib):LIBS += -L/opt/openssl/lib
    LIBS += -lssl -lcrypto
  }
  win32 {
    # Adapt 'C:\OpenSSL-Win32' to your local path
    SSL_WIN = "C:\\OpenSSL-Win32"
    INCLUDEPATH += $${SSL_WIN}\\include
    LIBS += -L$${SSL_WIN}\\bin -leay32
  }
}

# Doxygen
doxygen.target = doxygen
doxygen.commands = doxygen Doxyfile
QMAKE_EXTRA_TARGETS += doxygen
QMAKE_CLEAN += -r doxygen

SOURCES += \
  src/activity.cc \
  src/appenddialog.cc \
  src/main.cc \
  src/mainwindow.cc \
  src/milestone.cc \
  src/milestonedialog.cc \
  src/project.cc \
  src/setupdialog.cc \
  src/proinitdialog.cc \
  src/questionnairedialog.cc \
  src/logview.cc

HEADERS +=  \
  src/activity.h \
  src/appenddialog.h \
  src/definitions.h \
  src/mainwindow.h \
  src/milestone.h \
  src/milestonedialog.h \
  src/project.h \
  src/setupdialog.h \
  src/proinitdialog.h \
  src/questionnairedialog.h \
  src/logview.h

RESOURCES += \
  doc/doc.qrc \
  resources/img.qrc

OTHER_FILES += \
  resources/Info.plist \
  Doxyfile \
  README.md

DISTFILES += \
  tools/mac_osx_install.sh \
  tools/mac_osx_deploy.sh \
  tools/windows_deploy.bat \
  tools/windows_deploy_encrypted.bat

