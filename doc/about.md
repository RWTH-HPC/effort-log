<!--
  Copyright (c) 2015-2018 by IT Center, RWTH Aachen University

  This file is part of EffortLog, a tool for collecting software
  development effort.

  EffortLog is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  EffortLog is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with EffortLog.  If not, see <http://www.gnu.org/licenses/>.
-->

# About EffortLog

This help provides information about EffortLog.

## History of EffortLog

EffortLog was started in 2015 at the IT Center of RWTH Aachen
University. Its main purpose is to provide a simple program to aid
developers in tracking their programming efforts. The gained data can
than be evaluated for researching purposes on programming effort
analysis. Please see
[www.hpc.rwth-aachen.de/research/tco](http://www.hpc.rwth-aachen.de/research/tco)
for more information on the research done with EffortLog.

Change log:

* 0.9.4 (June 2018):
  * Added feature: Questionnaires remember characteristics of last performance measurement
  * Bug fix:               Improved scaling on high-DPI displays
* 0.9.3 (April 2018):
  * Added feature: Added a tray icon and desktop notifications
  * Added feature: The view on the questionnaire is now scrollable
  * Bug fix:               Improved cross-platform window displaying
* 0.9.2 (March 2018):
  * Added feature: Simplified set-up
* 0.9.1 (March 2018):
  * Added feature: Added auto-completion to most of the questionnaire forms
  * Bug fix:               Consistent scheduler IDs for the diary entries
* 0.9.0 (February 2018):
  * Added feature: Major reorganization of the questionnaires towards a single-page layout
* 0.8.3 (May 2017):
  * Bug fix:               Deployment reverted to Qt5.6 due to packaging issues with QT5.8
* 0.8.2 (May 2017):
  * Added feature: Support for changes to file directories for existing projects
  * Bug fix:               Skip read of log file on newly created projects
  * Bug fix:               Support for blanks in directory names
* 0.8.1 (January 2017):
  * Added feature: Added a viewer of the current log file during the questionnaire
* 0.8 (September 2016):
  * Added feature: Added a convenient view of the current log file sorted by dates
  * Added feature: Can now handle development sessions spanning multiple days
* 0.7 (February 2016):
  * Added feature: Encryption is disabled by default. Add \`-config crypt\` to your qmake flags to enable encryption
  * Bug fix:               Empty time stamps in the log files
* 0.6 (January 2016):
  * Added feature: Support for full 256-bit AES encryption of all project and log files
  * Bug fix:               Appending an event fails due to encryption
  * Bug fix:               Wrong interval length of the first logging event
* 0.5 (December 2015):
  * Added feature: Added milestones to projects
  * Added feature: Added IDs to each logging event
  * Bug fix:               Fixed US locale to unify data format of log files
  * Bug fix:               Milestones have mismatching IDs between the logging event and the current milestone
* 0.4 (December 2015):
  * Added feature: Configuration unified in one dialog
  * Added feature: Added appendix to log file at program startup
  * Bug fix:               General fixes
* 0.3 (November 2015):
  * Added feature: New configuration window
  * Added feature: Main window simplified
  * Bug fix:               Errors with the generated JSON files
* 0.2 (October 2015):
  * Bug fix:               Improved JSON handling
* 0.1 (September 2015): Initial release

## Needed Privileges to Run EffortLog

EffortLog can be run as any user as long as the user owns the rights to
write to the specified output directory of the log file. Note that users
on Unix operating systems might need to set the binary file to be
executable. The binary file is called "effort-log" on Linux operating
systems and can be set executable by the following command:
`chmod +x effort-log`.

## Licensing of EffortLog

Copyright © 2015-2018 IT Center, RWTH Aachen University

This project is licensed under version 3 of the [GNU General Public
License](http://www.gnu.org/licenses/).

## Help the Development of EffortLog

For more information on the research done with EffortLog and general
questions, please see
[www.hpc.rwth-aachen.de/research/tco](http://www.hpc.rwth-aachen.de/research/tco)
or contact [Sandra Wienke](mailto:wienke@itc.rwth-aachen.de) or [Julian
Miller](mailto:miller@itc.rwth-aachen.de).

For information regarding the development of EffortLog, please see the
[readme file on
Github](https://github.com/julianmi/effort-log#developing-and-contributing-to-effortlog)
or contact [Julian Miller](mailto:miller@itc.rwth-aachen.de).
