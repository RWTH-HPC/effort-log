/*
 * Copyright (c) 2015-2017 by IT Center, RWTH Aachen University
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

#include <QApplication>
#include <QButtonGroup>
#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QSettings>
#include <QSpinBox>
#include <QTimer>
#include <QVBoxLayout>

#include "mainwindow.h"

//! Class to handle user input on appending a logging entry
/*!
 * This class implements a questionnaire in form of a pop-up.
 * Its main purpose is to provide the user with a convient and as little as
 * possible intrusive way of inputting what he or she was working before
 * executing the program. This class will only be called if the user specifies
 * that he or she wants to append a logging entry.
 * \see Dialog
 */
class AppendDialog : public QDialog {
  Q_OBJECT
public:
  //! The main constructor.
  AppendDialog(MainWindow *window, Activity *act);
  //! Overrides the default accept() member.
  /*!
   * accept() is executed on successfully quitting the dialog. This function
   * calls all setters of Activity to store the user-provided information in
   * an Activity. On accepting this dialog SetupDialog gains focus.
   * \see Activity
   */
  void accept() Q_DECL_OVERRIDE;
  //! Overrides the default reject() member.
  /*!
   * reject() is executed on quitting the dialog before finishing inputting all
   * the necessary information about the activity. A warning will pop up which
   * may prevent the user from unintendedly quitting the dialog. In case the
   * user quits the dialog the current logging event will be skipped and no
   * appendix will be added. On rejecting this dialog SetupDialog gains focus.
   */
  void reject() Q_DECL_OVERRIDE;

 private slots:
  //! Updates the view of the dialog
  /*!
   * This method updates the displayed message of group_activities_ whenever the
   * user changes the number of minutes he or she wants to be logged.
   */
  void UpdateUI();

 private:
  //! Takes care of all connections
  /*!
   * This method sets up the needed connections of the dialog. Theser are:
   *  - log_interval_spin_box_ -> UpdateUI(): executed on changes to
   * log_interval_spin_box_
   *  - finish_button_ -> acceptaccept(): execute accept() if finish_button_ is
   * pressed
   *  - skip_button_ -> reject(): execute reject() if skip_button_ is pressed
   */
  void CreateConnections();
  //! Sets up the view of the dialog
  /*!
   * This method initilaizes all objects needed for visualization of the dialog.
   * All objects (widgets, layouts, etc.) are bind to main_layout_. Additionally
   * this methods sets up the keybindigs for quickly choosing an activity.
   *
   * \see AppendDialog::main_layout_
   */
  void Setup();
  Activity *activity_;  /**< An object of type Activity which holds the matching
                            activity to the dialog.*/
  QButtonGroup *activity_button_group_;  /**< Holds all buttons for choosing an
                                              activity.*/
  QPlainTextEdit *comment_box_;  /**< A box to input comments on an activity.*/
  QGroupBox *group_activities_;  /**< Holds activity_button_group_ with the
                                      label info_string_.*/
  QPushButton *finish_button_;  /**< Button to exit the dialog after finishing
                                     all mandatory input.*/
  QString info_string_;  /**< Holds the question on what the user was working
                              on. Is updated on changes to
                              log_interval_spin_box_.*/
  QVBoxLayout *main_layout_;  /**< The main layout of the dialog.*/
  MainWindow *main_window_;  /**< Holds a pointer to the main window.*/
  int no_logged_activities_;  /**< The number of successfully logged events.*/
  QPushButton *skip_button_;  /**< Button to exit the dialog without logging the
                                   event to the log file (skipping this event)*/
  Project *project_; /**< Holds a pointer to the current project. \see Project*/
  QSettings settings_;  /**< Holds a copy of the global settings of the
                             application.*/
  QSpinBox *log_interval_spin_box_;  /**< Lets the user specify the interval of
                                          development before executing the
                                          program.*/
};
