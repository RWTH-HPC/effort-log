<!--
  Copyright (c) 2015-2017 by IT Center, RWTH Aachen University

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

About EffortLog {#about-effortlog .chapter}
===============

This help provides information about EffortLog.

History of EffortLog {#history-of-effortlog .section}
--------------------

EffortLog was started in 2015 at the IT Center of RWTH Aachen
University. Its main purpose is to provide a simple program to aid
developers in tracking their programming efforts. The gained data can
than be evaluated for researching purposes on programming effort
analysis. Please see
[www.hpc.rwth-aachen.de/research/tco](http://www.hpc.rwth-aachen.de/research/tco)
for more information on the research done with EffortLog.

Change log:

-   0.8.1 (January 2017):
    -   Added a viewer of the current log file during the questionnaire
-   0.8 (September 2016):
    -   Added a convenient view of the current log file sorted by dates
    -   Can now handle development sessions spanning multiple days
-   0.7 (February 2016):
    -   Encryption is disabled by default. Add \`-config crypt\` to your
        qmake flags to enable encryption
    -   Fixed a bug leading to empty time stemps in the log files
-   0.6 (January 2016):
    -   Fixed a bug in the interval length of the first logging event
    -   Added full 256-bit AES encryption of all project and log files
    -   Fixed a bug on saving an appended event which was introduced by
        the encryption
-   0.5 (December 2015):
    -   Added milestones to projects
    -   Added IDs to each logging event
    -   Fixed US locale to unify data format of log files
    -   Milestones match ID of the logging event where the current
        milestone was specified
-   0.4 (December 2015):
    -   Configuration unified in one dialog
    -   Added appendix to log file at program startup
    -   General bug fixes
-   0.3 (November 2015):
    -   New configuration window
    -   Main window simplified
    -   Fixed errors with the generated JSON files
-   0.2 (October 2015):
    -   Improved JSON handling
-   0.1 (September 2015): Initial release

Needed Privileges to Run EffortLog {#needed-privileges-to-run-effortlog .section}
----------------------------------

EffortLog can be run as any user as long as the user owns the rights to
write to the specified output directory of the log file. Note that users
on Unix operating systems might need to set the binary file to be
executable. The binary file is called "effort-log" on Linux operating
systems and can be set executable by the following command:
`chmod +x effort-log`.

Licensing of EffortLog {#licensing-of-effortlog .chapter}
----------------------

Copyright © 2015-2017 IT Center, RWTH Aachen University

This project is licensed under version 3 of the [GNU General Public
License](http://www.gnu.org/licenses/).

Help the Development of EffortLog {#help-the-development-of-effortlog .chapter}
---------------------------------

For more information on the research done with EffortLog and general
questions, please see
[www.hpc.rwth-aachen.de/research/tco](http://www.hpc.rwth-aachen.de/research/tco)
or contact [Sandra Wienke](mailto:wienke@itc.rwth-aachen.de) or [Julian
Miller](mailto:miller@itc.rwth-aachen.de).

For information regarding the development of EffortLog, please see the
[readme file on
Github](https://github.com/julianmi/effort-log#developing-and-contributing-to-effortlog)
or contact Julian Miller (miller@itc.rwth-aachen.de).
