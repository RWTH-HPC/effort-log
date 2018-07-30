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
