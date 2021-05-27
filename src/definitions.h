/*
 * Copyright (c) 2015-2019 by IT Center, RWTH Aachen University
 *
 * This file is part of EffortLog, a tool for collecting software
 * development effort.
 *
 * EffortLog is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * EffortLog is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with EffortLog.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#define APP_NAME \
  "Effort Log" /**< Global string holding the application's name.*/
#define APP_VERSION  \
  "@PROJECT_VERSION@" /**< Global string holding the application's version.*/
#define MIN_LOG_INTERVAL                      \
  1 /**< Global integer value holding the     \
         minimum logging interval whichcan be \
         set through the wizard.*/
#define MAX_LOG_INTERVAL                         \
  180 /**< Global integer value holding the      \
           maximum logging interval which can be \
           set through the wizard.*/
#define NUM_ACTIVITIES                        \
  9 /**< Global integer value which specifies \
         the number of different activities.*/
#define VERBOSE                                  \
  0 /**< Global integer value which specifies if \
         the program should output debugging     \
         information (==1).*/
#define LOG_ON_EXIT_THRESHOLD                  \
  0.2 /**< Global double precision value which \
           specifies the delay on which to ask \
           again on exiting the program.*/
