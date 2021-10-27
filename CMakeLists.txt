#------------------------------------------------------------------------------#
#  Copyright (c) 2015-2021 by IT Center, RWTH Aachen University
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

cmake_minimum_required(VERSION 3.16.0)

project(effort-log VERSION 1.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic)
endif()

find_package(Qt6 COMPONENTS Core Widgets)
if (NOT Qt6_FOUND)
    find_package(Qt5 5.15 COMPONENTS Core Widgets REQUIRED)
endif()

set(EFFORT_LOG_SOURCES
    src/activity.cc
    src/main.cc
    src/mainwindow.cc
    src/project.cc
    src/setupdialog.cc
    src/proinitdialog.cc
    src/questionnairedialog.cc
    src/logview.cc
    src/qstform.cc
    resources/img.qrc)

set(EFFORT_LOG_LIBS PRIVATE Qt::Core Qt::Widgets)

set(MACOSX_BUNDLE_INFO_PLIST resources/Info.plist)
if (APPLE)
    set(EFFORT_LOG_SOURCES ${EFFORT_LOG_SOURCES} src/appnap.mm)
    set(EFFORT_LOG_LIBS ${EFFORT_LOG_LIBS} "-framework Foundation" "-framework ApplicationServices")
endif()

option(CRYPT "Build with encryption support" OFF)

if (CRYPT)
    find_package(OpenSSL REQUIRED)
    if (OpenSSL_FOUND)
        include_directories(${OPENSSL_INCLUDE_DIRS})
        link_directories(${OPENSSL_LIBRARIES})
        set(EFFORT_LOG_LIBS ${EFFORT_LOG_LIBS} OpenSSL::SSL OpenSSL::Crypto)
        add_definitions(-DCRYPT)
        message(STATUS "Using OpenSSL ${OPENSSL_VERSION}")
    else (OpenSSL_FOUND)
        message("OpenSSL is required to compile effort-log with encryption support")
    endif()
    set(EFFORT_LOG_SOURCES ${EFFORT_LOG_SOURCES} src/crypt.cc src/passworddialog.cc)
endif()

add_executable(effort-log WIN32 MACOSX_BUNDLE ${EFFORT_LOG_SOURCES})

target_link_libraries(effort-log ${EFFORT_LOG_LIBS})

# Doxygen
find_package(Doxygen)

if (DOXYGEN_FOUND)
    # set input and output files
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile.out)

    # request to configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message("Doxygen build started")

    # Note: do not put "ALL" - this builds docs together with application EVERY TIME!
    add_custom_target( docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM )
else (DOXYGEN_FOUND)
  message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)
