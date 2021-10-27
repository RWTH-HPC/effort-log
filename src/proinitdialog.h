/*
 * Copyright (c) 2015-2021 by IT Center, RWTH Aachen University
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

#include <QButtonGroup>
#include <QDialog>
#include <QDialogButtonBox>
#include <QLabel>
#include <QPlainTextEdit>
#include <QRadioButton>
#include <QSplitter>
#include <QVBoxLayout>

#include "project.h"

//! Class to initialize a project
/*!
 * This class provides a dialog which lets users set up a new project
 */
class ProInitDialog : public QDialog {
  Q_OBJECT
 public:
  //! The main constructor.
  /*!
   * \param[in] pro A pointer to the project to be initialized. \see Project
   */
  ProInitDialog(Project *pro);
  //! Overrides the default accept() member.
  /*!
   * accept() is executed on successfully quitting the dialog.
   */
  void accept() Q_DECL_OVERRIDE;
  //! Overrides the default reject() member.
  /*!
   * reject() is executed on quitting the dialog.
   */
  void reject() Q_DECL_OVERRIDE;

 private:
  //! Takes care of all connections
  /*!
   * This method sets up the needed connections of the dialog. Theser are:
   *  - buttons_ -> accept(): execute accept() if OK is pressed
   *  - buttons_ -> reject(): execute reject() if CLOSE is pressed
   */
  void CreateConnections();
  //! Sets up the view of the dialog
  /*!
   * This method initilaizes all objects needed for visualization of the dialog.
   * All objects (widgets, layouts, etc.) are bind to main_layout.
   */
  void Setup();

  QDialogButtonBox *buttons_; /**< Buttons OK and CLOSE at the buttom of the
                                   dialog.*/
  Project *project_;          /**< Holds a pointer to the affiliated project.*/
  QButtonGroup *stages_button_group_; /**< Group stages_buttons_.*/
  QRadioButton *stages_buttons_[5];   /**< Radio buttons to select the
                                           development stage of the project.*/
  QPlainTextEdit *comment_box_; /**< Input of comments about the project.*/
};
