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

Prerequisites:  

* Qt 5.4.0 or later
* On Windows: MinGW with g++ 4.7 or later
* On Mac OS X: latest Xcode
* On Linux: g++ 4.7 or later
* Latest OpenSSL

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
**Project file \*.pro (JSON format)**

| Information  |  Collected through |
|---|---|
| Title of the project  | user-provided during program startup  |
| Username (may contain multiple entries if users share project files)  |  user-provided during program startup |
| Output directory of the project file  | user-provided during program startup  |
| Output directory of the log file |  user-provided during program startup |
| Logging interval in minutes  | user-provided during program startup  |

**Log file \*.json (JSON format)**

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

**Submit a Bug Report**
- Bugs are tracked through [Github Issues](https://github.com/julianmi/effort-log/issues).
- Please describe the bug as detailed as possible including steps which reproduce the problem.

**Request a Feature**

- Feature requests are tracked through [Github Issues](https://github.com/julianmi/effort-log/issues).
- Please describe the feature as detailed as possible including the changes you like to see compared to the current behavior.

**Make a Pull Request**

- Open a new [GitHub pull request](https://github.com/julianmi/effort-log/pulls) with the patch.
- Please use detailed descriptions on the purpose of the patch.
- Include the according issue number if present.

For more information on how to develop and contribute to EffortLog, please contact
[Julian Miller](mailto:miller@itc.rwth-aachen.de)

## Change Log

* 0.8 (September 2016)
  * Added a convenient view of the current log file sorted by dates
  * Can now handle development sessions spanning multiple days
* 0.7 (February 2016)
  * Encryption is disabled by default. Add `-config crypt` to your qmake flags to enable encryption
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

## Related Publications

1. S. Wienke, J. Miller, M. Schulz, M.S. Müller: Development Effort Estimation in HPC. Paper accepted at International ACM/IEEE International Conference for High Performance Computing, Networking, Storage and Analysis (SC16), November 2016, Salt Lake City, UT, USA.

## Contact Us
[Sandra Wienke](mailto:wienke@itc.rwth-aachen.de)  
[Julian Miller](mailto:miller@itc.rwth-aachen.de)  
IT Center  
Group: High Performance Computing  
Division: Computational Science and Engineering  
RWTH Aachen University  
Seffenter Weg 23  
D 52074  Aachen (Germany)

## License

Copyright © 2016 IT Center, RWTH Aachen University  
This project is licensed under version 3 of the GNU General Public License.
