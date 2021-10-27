# EffortLog

A cross-platform tool for collecting metrics about programming efforts of software developers. EffortLog is developed at [IT Center, RWTH Aachen University](http://www.hpc.rwth-aachen.de/research/tco).

## Introduction

EffortLog is mainly developed for research purposes on development efforts of software developers in the High Performance Computing (HPC) domain and can be used to capture the complete development life-cycle of a HPC project. Moreover, EffortLog can be used to log personal and team development efforts as well.

EffortLog captures various metrics about programming efforts through interval-based questionnaires. A popup window guides the user in pre-defined intervals through the questionnaire. Additionally, the user can specify information on a project milestone which may have been reached during the last interval. Milestones denote goals along the life-time such as a first-parallel version. The user can specify the achieved performance of the current milestone and how it was obtained. To gather fine-grained performance increments over the life time of an HPC project, milestones can be reached repeatedly.

## Binaries

You can download the binaries from [Github releases](https://github.com/julianmi/effort-log/releases). There are currently four different binaries distributed:

* EffortLog_`<version>`_osx.dmg: For Mac OS X 10.7 and higher
* EffortLog_`<version>`_osx_encrypted.dmg: For Mac OS X 10.7 and higher with enabled encryption of log files
* EffortLog_`<version>`_win32.zip: 32-bit version for Windows
* EffortLog_`<version>`_win32_encrypted.zip: 32-bit version for Windows with enabled encryption of log files

## Build from Source

You can download the sources from [Github releases](https://github.com/julianmi/effort-log/releases). 

### EffortLog >= v1.0.0

EffortLog >= v1.0.0 supports Qt 5.15 and Qt6. OS specific requirements:

* On Windows: MinGW with g++ 4.7 or later, [install Qt](https://www.qt.io/download-qt-installer)
* On Mac OS X: latest Xcode, `brew install qt`
* On Linux: [Qt for Linux/X11](https://doc.qt.io/qt-6/linux.html)

Latest OpenSSL if encryption is required:

* On Windows: [install OpenSSL for Windows](https://wiki.openssl.org/index.php/Binaries)
* On Mac OS X: `brew install openssl`
* On Ubuntu/Debian: `sudo apt install libssl-dev`
* On OpenSUSE: `sudo zypper install libopenssl-devel`
* On Fedora: `sudo yum install openssl-devel`
* On Arch Linux: `sudo pacman -S openssl`

You can build EffortLog with

    cd $EFFORTLOG_DIRECTORY
    cmake -S . -B build
    cmake --build build --config Release (or --config Debug for development)
    make (or mingw32-make on Windows)

The encrypted version can be build with
    cmake -DCRYPT=ON -DOPENSSL_ROOT_DIR=\<path to OpenSSl root directory\> -S . -B build
    cmake --build build --config Release (or --config Debug for development)
    make (or mingw32-make on Windows)

To create the *Doxygen* documentation use

    make doxygen

### EffortLog < v1.0.0

EffortLog < v1.0.0 provides compatibility with older versions of Qt5 down to version 5.5.0. OS specific requirements:

* On Windows: MinGW with g++ 4.7 or later, [install Qt](https://www.qt.io/download-qt-installer)
* On Mac OS X: latest Xcode, `brew install qt5`
* On Ubuntu/Debian: `sudo apt install build-essential qt5-default`
* On OpenSUSE: `sudo zypper install libQt5Core-devel libQt5Widgets-devel`
* On Fedora: `sudo yum install qt5-devel`
* On Arch Linux: `sudo pacman -S qt`

Latest OpenSSL if encryption is required:

* On Windows: [install OpenSSL for Windows](https://wiki.openssl.org/index.php/Binaries)
* On Mac OS X: `brew install openssl`
* On Ubuntu/Debian: `sudo apt install libssl-dev`
* On OpenSUSE: `sudo zypper install libopenssl-devel`
* On Fedora: `sudo yum install openssl-devel`
* On Arch Linux: `sudo pacman -S openssl`

You can build EffortLog with

    cd $EFFORTLOG_DIRECTORY
    qmake (or qmake-qt5)
    make (or mingw32-make on Windows)

Installation ("make install") is not needed. To specify the installation directory use

    qmake PREFIX=$INSTALL_DIRECTORY

To compile the release version use

    qmake -config release

To compile the debug version use

    qmake -config debug

To compile the release version with encryption of use

    qmake -config release -config crypt

To compile the debug version with encryption of use

    qmake -config debug -config crypt

To create the *Doxygen* documentation use

    make doxygen

## Information Gathered by EffortLog

### Project file \*.pro (JSON format)

| Information  |  Collected through |
|---|---|
| Title of the project  | user-provided during program startup  |
| Username (may contain multiple entries if users share project files)  |  user-provided during program startup |
| Output directory of the project file  | user-provided during program startup  |
| Output directory of the log file |  user-provided during program startup |
| Logging interval in minutes  | user-provided during program startup  |

### Log file \*.json (JSON format)

| Information  |  Collected through |
|---|---|
| Title of the project  | user-provided during program startup  |
| Username  |  user-provided during program startup |
| Logging interval specified at program startup  | user-provided during program startup  |
| Initial stage of the project  | user-provided during project initialization  |
| Comment on the initial stage of the project if provided  |  user-provided during project initialization |
| Type of the activity  | user-provided during questionnaires  |
| ID of the logging event | Starting at 0, counting forwards  |
| Time of the logging event  | system time  |
| Time of the last logging event  | system time  |
| Actual logging interval  | current time - time of last event  |
| Comment on the activity if provided | user-provided during questionnaires |
| Number of events in the current logging session  | Starting at 1, counting forwards  |
| Scheduler (1 denotes an interval-based event , 2 an appeneded event, 3 a manual event executed through the GUI, and 4 an event on  the program)  | Inserted by the tool  |
| Title of the milestone  | user-provided during questionnaires  |
| ID of the milestone (starting at 0)  | user-provided during questionnaires  |
| Comment on the milestone if provided  | user-provided during questionnaires  |
| Used performance metric  | user-provided during questionnaires  |
| Reached performance  | user-provided during questionnaires  |
| Used architecture for the performance measurements  | user-provided during questionnaires  |
| Number of threads, nodes, etc. for the performance measurements  | user-provided during questionnaires  |
| Used compiler  | user-provided during questionnaires  |
| Used programming model  | user-provided during questionnaires |

Notes on anonymity:

* EffortLog does not store any information on its users except of the project file (\*.pro) and the log file (\*.json). These files contain only user-provided input obtained from the questionnaires.
* Use random usernames.
* The project file (\*.pro) may contain the username of the system you are currently logged in. Please check this file before sharing it.
* The times of each event and milestone are logged by default. They can be removed by deleting the keys `CurLoggingTime`, `LastLoggingTime` and `Time` from the JSON files.

## Encryption

EffortLog can be configured to encrypt all user related files to ensure the safety and privacy of its users. All project files and log files are then encrypted with a strong 256-bit AES encryption. The user sets a password for each project on project-setup. To manually decrypt the log files a current version of [OpenSSL](https://www.openssl.org)  is required. Decryption can be done via command-line with the following command:

    openssl enc -aes-256-cbc -d -in <encrypted log file>.json -out <decrypted log file>

Notes:

* Encryption is disabled by default. Add `-config crypt` to your qmake flags to enable encryption.
* Choosing a complex and unique password for each project is crucial to ensure privacy!
* As of January 2016, changing passwords for a project is not possible once the password is set.

## Developing and Contributing to EffortLog

### Submit a Bug Report

* Bugs are tracked through [Github Issues](https://github.com/julianmi/effort-log/issues).
* Please describe the bug as detailed as possible including steps which reproduce the problem.

### Request a Feature

* Feature requests are tracked through [Github Issues](https://github.com/julianmi/effort-log/issues).
* Please describe the feature as detailed as possible including the changes you like to see compared to the current behavior.

### Make a Pull Request

* Open a new [GitHub pull request](https://github.com/julianmi/effort-log/pulls) with the patch.
* Please use detailed descriptions on the purpose of the patch.
* Include the according issue number if present.

For more information on how to develop and contribute to EffortLog, please contact
[Julian Miller](mailto:miller@itc.rwth-aachen.de)

## Change log

* 1.0.0 (May 2021):
  * Added feature:      Support for Qt 6
  * Chore:              Switch to cmake
* 0.9.6 (March 2021):
  * Chore:              Updated to Qt 5.15
  * Chore:              Removed some deprecated code
  * Bug fix:            Add ctrl key to hotkeys to prevent interference with user input
  * Bug fix:            Negative time in the header when logging across date borders
* 0.9.5 (January 2019):
  * Bug fix:            Missing visual updates to project settings
  * Bug fix:            Wrong type when importing project files
  * Bug fix:            Segfault when pressing "Log current efforts"
* 0.9.4 (June 2018):
  * Added feature:      Questionnaires remember characteristics of last performance measurement
  * Bug fix:            Improved scaling on high-DPI displays
* 0.9.3 (April 2018):
  * Added feature:      Added a tray icon and desktop notifications
  * Added feature:      The view on the questionnaire is now scrollable
  * Bug fix:            Improved cross-platform window displaying
* 0.9.2 (March 2018):
  * Added feature:      Simplified set-up
* 0.9.1 (March 2018):
  * Added feature:      Added auto-completion to most of the questionnaire forms
  * Bug fix:            Consistent scheduler IDs for the diary entries
* 0.9.0 (February 2018):
  * Added feature:      Major reorganization of the questionnaires towards a single-page layout
* 0.8.3 (May 2017):
  * Bug fix:            Deployment reverted to Qt5.6 due to packaging issues with QT5.8
* 0.8.2 (May 2017):
  * Added feature:      Support for changes to file directories for existing projects
  * Bug fix:            Skip read of log file on newly created projects
  * Bug fix:            Support for blanks in directory names
* 0.8.1 (January 2017):
  * Added feature:      Added a viewer of the current log file during the questionnaire
* 0.8 (September 2016):
  * Added feature:      Added a convenient view of the current log file sorted by dates
  * Added feature:      Can now handle development sessions spanning multiple days
* 0.7 (February 2016):
  * Added feature:      Encryption is disabled by default. Add \`-config crypt\` to your qmake flags to enable encryption
  * Bug fix:            Empty time stamps in the log files
* 0.6 (January 2016):
  * Added feature:      Support for full 256-bit AES encryption of all project and log files
  * Bug fix:            Appending an event fails due to encryption
  * Bug fix:            Wrong interval length of the first logging event
* 0.5 (December 2015):
  * Added feature:      Added milestones to projects
  * Added feature:      Added IDs to each logging event
  * Bug fix:            Fixed US locale to unify data format of log files
  * Bug fix:            Milestones have mismatching IDs between the logging event and the current milestone
* 0.4 (December 2015):
  * Added feature:      Configuration unified in one dialog
  * Added feature:      Added appendix to log file at program startup
  * Bug fix:            General fixes
* 0.3 (November 2015):
  * Added feature:      New configuration window
  * Added feature:      Main window simplified
  * Bug fix:            Errors with the generated JSON files
* 0.2 (October 2015):
  * Bug fix:            Improved JSON handling
* 0.1 (September 2015): Initial release

## Related Publications

1. S. Wienke, J. Miller, M. Schulz, M.S. Müller: Development Effort Estimation in HPC. International ACM/IEEE International Conference for High Performance Computing, Networking, Storage and Analysis (SC16), November 2016, Salt Lake City, UT, USA.
2. J. Miller: Software Cost Estimation for the Development Effort applied to Multi-node GPU Aeroacoustics Simulations. Master Thesis, 2016, RWTH Aachen University, Germany.
3. J. Miller, S. Wienke, M. Schlottke-Lakemper, M. Meinke, and M. S. Müller: Applicability of the Software Cost Model COCOMO II to HPC Projects. International Journal of Computational Science and Engineering 2018 17:3, 283-296.

## Contact Us

[Julian Miller](mailto:miller@itc.rwth-aachen.de)
[Sandra Wienke](mailto:wienke@itc.rwth-aachen.de)
IT Center
Group: High Performance Computing
Division: Computational Science and Engineering
RWTH Aachen University
Seffenter Weg 23
D 52074  Aachen (Germany)

## License

Copyright © 2015-2021 IT Center, RWTH Aachen University
This project is licensed under version 3 of the GNU General Public License.
