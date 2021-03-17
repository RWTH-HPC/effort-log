#!/bin/sh

echo
echo "Setup build environment"
case "$1" in
    *"ubuntu"*)
        apt -y update && apt -y install git build-essential qt5-default libssl-dev
        ;;
    *"debian"*)
        apt -y update && apt -y install git build-essential qt5-default libssl-dev
        ;;
    *"fedora"*)
        yum -y update && yum -y install git make qt5-devel openssl-devel
        ;;
    *"opensuse"*)
        zypper -n update && zypper -n install git libQt5Core-devel libQt5Widgets-devel libopenssl-devel
        ;;
    *"archlinux"*)
        pacman -Syu --noconfirm && pacman -S --noconfirm git gcc make qt5-base openssl
        ;;
    *)
        echo "Unknown OS"
        exit 1
esac

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

$QMAKE --version
if [ -n "$CPP" ]; then
    $CPP --version
fi

cd /effort-log || { echo "No directory /effort-log"; exit 1; }

echo
echo "Building debug version"
"${QMAKE}" -config debug || { echo "Failed configuring debug version"; exit 1; }
make || { echo "Failed building debug version"; exit 1; }
make clean || { echo "Failed clean-up of debug version"; exit 1; }

echo
echo "Building release version"
"${QMAKE}" -config release || { echo "Failed configuring release version"; exit 1; }
make || { echo "Failed building release version"; exit 1; }
make clean || { echo "Failed clean-up of release version"; exit 1; }

echo
echo "Building debug version with encryption support"
"${QMAKE}" -config debug -config crypt || { echo "Failed configuring debug version with encryption support"; exit 1; }
make || { echo "Failed building debug version with encryption support"; exit 1; }
make clean || { echo "Failed clean-up of debug version with encryption support"; exit 1; }

echo
echo "Building release version with encryption support"
"${QMAKE}" -config release -config crypt || { echo "Failed configuring release version with encryption support"; exit 1; }
make || { echo "Failed building release version with encryption support"; exit 1; }
make clean || { echo "Failed clean-up of release version with encryption support"; exit 1; }
