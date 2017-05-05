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
#include <QDialog>
#include <QDir>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QFormLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QMessageBox>
#include <QSpinBox>
#include <QPushButton>

#include "mainwindow.h"
#include "appenddialog.h"
#ifdef CRYPT
#include "crypt.h"
#endif

//! Class to setup the program
/*!
 * This class implements a dialog which lets the user specify the settings
 * of the program. This dialog is executed once on program startup.
 */
class SetupDialog : public QDialog {
  Q_OBJECT
public:
  //! The main constructor.
  /*!
   * \param[in] window A pointer to the main window.
   */
  SetupDialog(MainWindow *window);
  //! The main constructor with encryption.
  /*!
   * \param[in] window A pointer to the main window.
   * \param[in] crypt Handles encryption and decryption for the project
   */
#ifdef CRYPT
  SetupDialog(MainWindow *window, Crypt *crypt);
#endif
  //! Overrides the default accept() member.
  /*!
   * accept() is executed on successfully quitting the dialog. This function
   * sets up all global variables stored in QSettings. Additionally, the
   * MainWindow is prepared and is executed after accepting this dialog.
   * \see QSettings, MainWindow
   */
  void accept() Q_DECL_OVERRIDE;
  //! Overrides the default reject() member.
  /*!
   * reject() is executed on quitting the dialog before finishing inputting all
   * the necessary settings needed to run this program. A warning will pop up
   * which may prevent the user from unintendedly quitting the dialog. In case
   * the user quits the dialog the main program is exited aswell.
   */
  void reject() Q_DECL_OVERRIDE;

  Project *project_; /**< Holds a pointer to the current project. \see Project*/

public slots:

private:
  //! Sets up the view of the dialog
  /*!
   * This method initilaizes all objects needed for visualization of the dialog.
   * All objects (widgets, layouts, etc.) are bind to layout_. Additionally,
   * this method loads all settings the user provided on lastly executing this
   * program. If the program is run for the first time defualt values are
   * displayed.
   * \see SetupDialog::layout_
   */
  void Setup();
  //! Takes care of all connections
  /*!
   * This method sets up the needed connections of the dialog. Theser are:
   *  - browse_button_ -> BrowseDir(): execute BrowseDir() if browse_button is
   * pressed
   *  - app_button_ -> Append(): execute Append() if app_button_ is
   * pressed
   *  - project_title_line_edit_ -> SetLogFileName(): execute SetLogFileName()
   * if project_title_line_edit_ is changed
   *  - user_name_line_edit_ -> SetLogFileName(): execute CheckInput()
   * if user_name_line_edit_ is changed
   *  - project_title_line_edit_ -> SetLogFileName(): execute CheckInput()
   * if project_title_line_edit_ is changed
   *  - log_file_dir_line_edit_ -> SetLogFileName(): execute CheckInput()
   * if log_file_dir_line_edit_ is changed
   *  - log_file_name_line_edit_ -> SetLogFileName(): execute CheckInput()
   * if log_file_name_line_edit_ is changed
   *  - buttons_ -> accept(): execute accept() if accepted() is
   * pressed
   *  - buttons_ -> reject(): execute reject() if rejected() is pressed
   */
  void CreateConnections();
  //! Loads program settings
  /*!
   * This method loads all settings stored by SetupDialog::SaveSettings() on
   * the last successfull dialog exit (call to accept())
   * \see QSettings
   */
  void LoadSettings();
  //! Stores program settings
  /*!
   * This method stores all settings provided by the user on accepting the
   * dialog.
   * \see QSettings
   */
  void SaveSettings();
#ifdef CRYPT
  Crypt *crypt_;
#endif
  MainWindow *main_window_;  /**< Holds a pointer to the main window.*/
  QGridLayout *layout_;  /**< The main layout of the dialog.*/
  QString project_dir_;  /**< Holds a string of the project's working
                              directory.*/
  QDialogButtonBox *buttons_;  /**< Buttons to accept or quit the dialog.*/
  QPushButton *browse_log_button_;  /**< Button to browse a directory.*/
  QPushButton *app_button_;  /**< Buttons to append a logging entry.*/
  QLineEdit *project_title_line_edit_;   /**< QLineEdit to edit the project's
                                              title.*/
  QLineEdit *user_name_line_edit_;   /**< QLineEdit to edit the user name.*/
  QSpinBox *log_interval_spin_box_;   /**< QSpinBox to set the interval of
                                           development before executing the
                                           program.*/
  QSettings settings_;  /**< Holds a copy of the global settings of the
                             application.*/
  QLineEdit *log_file_name_line_edit_;   /**< QLineEdit to edit the log file
                                          name.*/
  QLineEdit *log_file_dir_line_edit_;  /**< QLineEdit to edit the log file's
                                          directory*/
  QString dir_string_;   /**< A string which holds the log file's directory.*/
  QButtonGroup *stages_button_group_;  /**< Buttons to specify the current
                                            project's development stage.*/

  QPushButton *browse_pro_button_;  /**< Button to browse to a working
                                         directory of the project.*/
  QLineEdit *project_dir_line_edit_;   /**< QLineEdit to edit the working
                                            directory of the project.*/

  bool new_project_flag_;   /**< Indicates if the project already exists.*/
  QStringList dir_list_;   /**< A list of working directories.*/
  QCompleter *completer_;   /**< Auto-completion of the title of known
                                 projects.*/
  Activity *append_activity_;   /**< An activity to append to the current
                                     project.*/

private slots:
  //! Browse a project file
  /*!
   * This method opens a dialog which lets the user browse for a project file
   * (*.pro) contraining information about a previously saved project.
   */
  void BrowsePro();
  //! Browse a directory
  /*!
   * This method opens a dialog which lets the user browse a directory. This
   * directory is the working directory throughout the execution of the
   * program.
   */
  void BrowseDir();
  //! Sets the text of log_file_name_line_edit_
  /*!
   * This method sets the text of log_file_name_line_edit_ to the user-provided
   * text in project_title_line_edit_.
   */
  void SetLogFileName();
  //! Checks the provided settings
  /*!
   * This method checks the provided settings. If all settings are specified
   * correctly the button OK is enabled
   */
  void CheckInput();
  //! Executes AppendDialog()
  /*!
   * This method creates a new dialog of Class AppendDialog which gains focus
   * over the current SetupDialog.
   */
  void Append();
  //! Handles the user-specified project
  bool ProjectHandler();
  //! Auto-completion of the title of known projects
  void AutoComplete();
};
