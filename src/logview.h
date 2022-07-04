/*
 * Copyright (c) 2015-2022 by IT Center, RWTH Aachen University
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

#include <QDialog>
#include <QStandardItemModel>
#include <QTreeView>
#include "project.h"

//! Class to view to current log file
/*!
 * This class implements a tree view of the current log file sorted by dates.
 * \see Dialog
 */
class LogView : public QDialog {
  Q_OBJECT
 public:
  //! The main constructor.
  LogView(Project *pro);
};
