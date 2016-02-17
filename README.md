# EffortLog

A cross-platform tool for collecting metrics about programming efforts of software developers. EffortLog is developed at [IT Center, RWTH Aachen University](https://www.itc.rwth-aachen.de/go/id/eubj).

## Introduction

EffortLog is mainly developed for research purposes on development efforts of software developers in High Performance Computing. This includes all sorts of parallelizing and tuning of source code. However, EffortLog may be used to log personal and team development efforts as well.

EffortLog captures various metrics about programming efforts through interval-based questionnaires. The time span of the interval is specified at program startup. A popup window guides the user through several input fields of various information (see next section) on the developer's activity during the last interval. Additionally, the user can specify information on a project milestone which may be reached during the last interval.

## Information gathered by EffortLog
* Project file \*.pro (JSON format)

  | Information  |  Collected through |
  |---|---|
  | Title of the project  | user-provided during program startup  |
  | Username (may contain multiple entries if users share project files)  |  user-provided during program startup |
  | Output directory of the project file  | user-provided during program startup  |
  | Output directory of the log file |  user-provided during program startup |
  | Logging interval in minutes  | user-provided during program startup  |

* Log file \*.json (JSON format)

  | Information  |  Collected through |
  |---|---|
  | Initial stage of the project  | user-provided during project initialization  |
  | Comment on the initial stage of the project if provided  |  user-provided during project initialization |
  | Type of the activity  | user-provided during questionnaires  |
  | Comment on the activity |  user-provided during questionnaires |
  | Time of the logging event  | system time  |
  | Time of the last logging event  | system time  |
  | ID of the logging event | Starting at 0, counting forwards  |
  | Number of events in the current logging session  | Starting at 1, counting forwards  |
  | Logging interval specified at program startup  | user-provided during program startup  |
  | Actual logging interval  |current time - time of last event  |
  | Title of the project  | user-provided during program startup  |
  | Username  |  user-provided during program startup |
  | Title of the milestone  | user-provided during questionnaires  |
  | ID of the milestone (starting at 0)  | user-provided during questionnaires  |
  | Matching ID of the logging event during which the user specified the milestone | user-provided during questionnaires  |
  | Comment on the used performance metric  | user-provided during questionnaires  |
  | Comment on the reached performance  | user-provided during questionnaires  |
  | Comment on the used architecture for the performance measurements  | user-provided during questionnaires  |
  | Comment on the number of threads, nodes, etc. for the performance measurements  | user-provided during questionnaires  |
  | Comment on the used compiler  | user-provided during questionnaires  |
  | Comment on the used programming model  | user-provided during questionnaires  |
  | Other comment on the milestone if provided  | user-provided during questionnaires  |

Notes on anonymity:
* EffortLog does not store any information on its users except of the project file (\*.pro) and the log file (\*.json). These files contain only user-provided input obtained from the questionnaires.
* Use random usernames
* The project file (\*.pro) may contain the username of the system you are currently logged in. Please check this file before sharing it.

## How this information is stored safely by EffortLog
EffortLog can be configured to encrypt all user related files to ensure the safety and privacy of its users. All project files and log files are then encrypted with a strong 256-bit AES encryption. The user sets a password for each project on project-setup. To manually encrypt the log files a current version of [OpenSSL](https://www.openssl.org)  is required. Encryption can be done via command-line with the following command:
    `openssl enc -aes-256-cbc -d -in <decrypted file>.json -out encrypted_file`
Notes:
* Encryption is disabled by default. Add `-config crypt` to your qmake flags to enable encryption.
* Choosing a complex and unique password for each project is crucial to ensure privacy!
* As of January 2016, changing passwords for a project is not possible once the password is set.


## Installation from Source

Prerequisites:

* Qt 5.4.0 or later
* On Windows: MinGW with g++ 4.7 or Visual Studio 2013 Update 2 or later
* On Mac OS X: latest Xcode
* On Linux: g++ 4.7 or later
* Latest OpenSSL

You can build EffortLog with

    cd $EFFORTLOG_DIRECTORY
    qmake
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

## Developing and Contributing to EffortLog

For more information on how to develop and contribute to EffortLog, please contact
[Julian Miller](mailto:miller@itc.rwth-aachen.de)

## Change Log

* 0.7 (February 2016)
  * Encryption is disabled by default. Add `-config crypt` to your qmake flags to enable encryption.
  * Fixed a bug leading to empty time stemps in the log files
* 0.6 (January 2016)
  * Fixed a bug in the interval length of the first logging event
  * Added full 256-bit AES encryption of all project and log files
  * Fixed a bug on saving an appended event which was introduced by the encryption
* 0.5 (December 2015)
  * Added milestones to projects
  * Added IDs to each logging event
  * Milestones match ID of the logging event where the current milestone was specified
  * Fixed US locale to unify data format of log files
* 0.4 (December 2015)
  * Configuration unified in one dialog
  * Added appendix to log file at program startup
  * General bug fixes
* 0.3 (November 2015)
  * New configuration window
  * Main window simplified
  * Fixed errors with the generated JSON files
* 0.2 (October 2015)
  * Improved JSON handling
* 0.1 (September 2015):
  * Initial release

## Contact

For more information on the research done with EffortLog and general questions, please contact [Sandra Wienke](mailto:wienke@itc.rwth-aachen.de).

For information regarding the development of EffortLog, please contact [Julian Miller](mailto:miller@itc.rwth-aachen.de).


## License

Copyright © 2016 IT Center, RWTH Aachen University

This project is licensed under version 3 of the GNU General Public License.
