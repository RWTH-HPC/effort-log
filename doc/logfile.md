<!--
  Copyright (c) 2015-2022 by IT Center, RWTH Aachen University

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

# Log File

This help describes the log file which holds all information collected
by EffortLog. Its main purpose is to further evaluate the developer's
effort.

## Format and Structure of the Log File

The log file is formatted in JSON. It contains of the following entries:

-   "InitialProjectStage": The initial development stage of the project.
    This was specified during the configuration wizard. The choices for
    the stage are:
    -   Scratch
    -   Working serial version
    -   Tuned serial version
    -   Working parallel version
    -   Tuned parallel version
-   "InitialProjectStageComment": A user-provided comment on the above
    development stage.
-   "LoggingEvents": A list of the logging events sorted by the
    occurrence. Each entry consists of the following keys:
    -   "ID": A unique identifier of the event. The first event holds id
        0 and all following events are then consecutively numbered.
    -   "UserName": The user's name. This was specified during the
        configuration wizard.
    -   "ProjectTitle": The title of the project. This was specified
        during the configuration wizard.
    -   "ProjectStage": The current development stage of the project.
        This was specified during the configuration wizard. The choices
        for the stage are:
        -   Scratch
        -   Working serial version
        -   Tuned serial version
        -   Working parallel version
        -   Tuned parallel version
    -   "ActivityType": The user-specified type (free-form) of activity.
        The user is provided with a list of default activities:
        -   Break
        -   Thinking
        -   Serial
        -   Parallelizing
        -   Testing
        -   Debugging
        -   Tuning
        -   Experimenting
    -   "Comment": Contains a comment on the activity. If the user did
        not enter any comment this field is empty.
    -   "CurLoggingTime": The time when the user finished the
        questionnaire.
    -   "Interval": The time in minutes since the last stored logging
        event.
    -   "LastLoggingTime": The time when the user finished the
        questionnaire the time before the current event.
    -   "LoggingInterval": The specified logging interval. This was
        specified during the configuration wizard.
    -   "NoEventsCurrentSession": A count of the number of events logged
        during the current session.
    -   "Scheduler": Specifies how the event was triggered. -1 denotes
        an unknown or undefined event, 0 denotes an interval-based event
        , 1 an appended event, 2 a manual event executed through the
        GUI, and 3 an event on closing the program.
    -   "Architecture": The architecture used during the performance
        measurements.
    -   "Compiler": The compiler used for generating the binary which
        was executed for the performance measurements.
    -   "DataSize": The problem size used during the performance
        measurements.
    -   "NoThreadsNodes": The number of threads/nodes which were
        utilized for the performance measurements.
    -   "PerfComment": The obtained performance measure.
    -   "PerfMetric": The performance metric used.
    -   "ProgrammingModel": The programming model used.
    -   "ThreadsNodes": Distinguishes if threads or nodes are used for
        the key NoThreadsNodes.
    -   "MsID": A unique identifier of the milestone. The first event
        holds id 0 and all following events are then consecutively
        numbered. The ID is -1 if no milestone was specified.
    -   "MSTitle": The user-defined type (free-form) of milestone. The
        default choices include:
        -   Working serial version
        -   Tuned serial version
        -   Working parallel version
        -   Tuned parallel version
    -   "MsComment": Contains a comment on the milestone. If the user
        did not enter any comment this field is empty.

Note: The JSON keys are typically alphabetically ordered by the Qt JSON
library. Note 2: The tool expects unaltered JSON files. Do not change
the log files manually!

## Handling of the Log File

There are two different ways how EffortLog handles the log file:

-   If the specified log file does not exist EffortLog will create it
    and start writing to it.
-   If the specified log file does exist EffortLog will append to the
    end of file. This way, old logs are never overwritten.

To further evaluate the log file external software can be used. There
are several programs which are able to read JSON files as well as
interfaces to most of the programming languages.
