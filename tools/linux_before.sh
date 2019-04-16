#!/bin/sh

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
        pacman -Syu --noconfirm && pacman -S --noconfirm git gcc make qt openssl
        ;;
    *)
        echo "Unknown OS"
esac
