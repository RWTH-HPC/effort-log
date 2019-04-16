#!/bin/sh
set -e

QMAKE=/usr/local/Cellar/qt/5.12.2/bin/qmake
if [ -z "$QMAKE" ]; then
    QMAKE=$(command -v qmake 2>&1)
    if [ -z "$QMAKE" ]; then
        QMAKE=$(command -v qmake-qt5 2>&1)
        if [ -z "$QMAKE" ]; then
            echo "Error: qmake was not found."
            exit 1
        fi
    fi
fi

echo
echo "Build environment"
$QMAKE --version
if [ -n "$CPP" ]; then
    $CPP --version
fi

echo
echo "Building debug version"
"${QMAKE}" -config debug
make -j
make clean

echo
echo "Building release version"
"${QMAKE}" -config release
make -j
make clean

echo
echo "Building debug version with encryption support"
"${QMAKE}" -config debug -config crypt
make -j
make clean

echo
echo "Building release version with encryption support"
"${QMAKE}" -config release -config crypt
make -j
make clean
