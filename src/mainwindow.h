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

#include <QAction>
#include <QFrame>
#include <QHeaderView>
#include <QLCDNumber>
#include <QLabel>
#include <QLayout>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QProgressBar>
#include <QSettings>
#include <QSplitter>
#include <QStandardItemModel>
#include <QStatusBar>
#include <QString>
#include <QTableView>
#include <QTextBrowser>
#include <QTime>
#include <QTimer>
#include <QToolBar>
#include <QUrl>

#include "crypt.h"
#include "project.h"

//! Class for dislaying the main window
/*!
 * This class implements the main view of the program. Its main purpose is to
 * display information during the run-time of the program. It consists of a
 * simple message showing the specified logging interval and the choosen user
 * name aswell as the project's title. Additionally a timer until the next
 * scheduled logging event is shown.
 */
class MainWindow : public QMainWindow {
  Q_OBJECT
public:
  //! The main constructor.
  explicit MainWindow();
  //! The main constructor with encryption
  /*!
   * \param[in] crypt Handles encryption and decryption for the project
   */
  explicit MainWindow(Crypt *crypt);
  //! Overrides the default closeEvent() member.
  /*!
   * closeEvent() is executed on quitting the main window. A warning will pop up
   * which may prevent the user from unintendedly quitting the main program and
   * therefor stop the logging. If the user decides to exit the main program
   */
  void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
  //! Updates the view of the main program
  /*!
   * This method is called when the setup wizard was sucessfully exited. It
   * sets the displayed message providing information about the user-specified
   * settings of the program. Moreover the main window is set to a fixed size
   * since resizing it is unnecessary.
   */
  void UpdateUI();
  //! Sets log_running_
  /*!
   * This method is sets the variable log_running_ with the given bool r.
   * \param[in] r Specifies if the program is running
   *
   * \see MainWindow::log_running_
   */
  void SetLogRunning(bool r);
  //! Sets up animation in the status bar of MainWindow
  /*!
   * This method initilaizes all objects needed for the animation of the time
   * until the next scheduled logging event in the status bar of MainWindow.
   *
   * \see MainWindow::status_bar_
   */
  void SetupAnimation();
  //! Sets status_label_time_
  /*!
   * This method is sets the variable status_label_time_ with the given QString
   * s.
   * \param[in] s Time to display in the statusbar
   * \see MainWindow::status_label_time_
   */
  void SetTimeLabel(QString s);
  //! Call to bool Project::ReadLog(QString f)
  /*!
   * This method calls bool Project::ReadLog(QString f) on the locally stored
   * project.
   *
   * \see bool Project::ReadLog(QString f)
   */
  void ReadLog();
  //! Sets project_
  /*!
   * This method is sets the variable status_label_time_ with the given QString
   * s.
   * \param[in] p Pointer to a Project
   * \see Project
   */
  void SetProject(Project *p);

  Project *project_; /**< Holds a pointer to the current project. \see Project*/
public slots:
  void QuitOnSignal();

private slots:
  //! Updates the view of the countdown timer
  /*!
   * This method is called every second to update the view of the timer until
   * the next scheduled logging event.
   */
  void NextAnimationFrame();
  //! Displays a simple viewer for the created log files
  /*!
   * This method is called if the user executes log_action_, meaning the user
   * hits the "Browse current log file" button in the main menu of the program
   * located under "File". This methods lets the user specify a log file which
   * is displayed in plain text.
   *
   * \see MainWindow::log_action_
   */
  void LogViewer();
  //! Sets up a about message in the main menu of the program
  /*!
   * This method is called if the user executes about_action_, meaning the user
   * hits the "about" button in the main menu of the program located under
   * "help". The about message consists of a simple view of the program's
   * version and the author of the program.
   *
   * \see MainWindow::about_action_
   */
  void About();
  //! Sets up an help view in the main menu of the program
  /*!
   * This method is called if the user executes help_action_, meaning the user
   * hits the "help" button in the main menu of the program located under
   * "help". The help message consists of a view of a number of html files
   * providing help to the user of the program.
   *
   * \see MainWindow::help_action_
   */
  void Help();
  //! Manually executes the questionnaire
  /*!
   * This method is called if the user executes new_action_, meaning the user
   * hits the "new" button in the toolbar of the program. A new questionnaire is
   * executed in form of a dialog.
   */
  void ExecQuestionnaireDialog();
  //! Executes the questionnaire based on the set interval
  /*!
   * This method is called if the user executes new_action_, meaning the user
   * hits the "new" button in the toolbar of the program. A new questionnaire is
   * executed in form of a popup dialog.
   */
  void ExecScheduledQuestionnaireDialog();

private:
  //! Sets up the actions of the main program
  /*!
   * This method is called to initialize the actions in the menu of the program.
   * Note: Only call this method once to initialize the actions.
   */
  void CreateActions();
  //! Sets up the main menu of the program
  /*!
   * This method is called to initialize the main menu of the program. Note:
   * Only call this method once to initialize the main menu.
   */
  void CreateMenus();
  //! Sets up the connections of the main program
  /*!
   * This method is called to connect the actions in the menu of the program to
   * a matching slot. Note: Only call this method once to initialize the
   * connections. Theser are:
   *  - countdown_timer -> NextAnimationFrame(): executed every second
   *  - help_action_ -> Help(): executed on hitting "help" in the main menu
   *  - about_action_ -> About(): executed on hitting "about" in the main menu
   */
  void CreateConnections();
  //! Sets up the view of the main window
  /*!
   * This method initilaizes all objects needed for visualization of the main
   * window.
   * All objects (widgets, layouts, etc.) are bind to main_layout_.
   *
   * \see MainWindow::main_layout_
   */
  void Setup();

  Crypt *crypt_;
  QAction *about_action_;   /**< The action upon hitting the "about" button of
                                 the main menu.*/
  QWidget *central_widget_; /**< The main widget of the main window.*/
  QTime countdown_time_;    /**< Holds the time until the next logging event. Is
                                 updated every second.*/
  QTimer *countdown_timer_; /**< The countdown timer holding the value of
                                countdown_time_.*/
  QAction *help_action_;    /**< The action upon hitting the "help" button of
                                 the main menu.*/
  QLabel *info_label_;      /**< The label holding info_string_.*/
  QString info_string_;     /**< The user-provided information about the running
                                logging.*/
  QLCDNumber *lcd_;         /**< The display of the countdown timer in a LCD .*/
  QAction *log_action_; /**< The action upon hitting the "Browse file" button
                             of the main menu.*/
  QVBoxLayout *main_layout_; /**< The main layout of the main window.*/
  QAction *new_action_;      /**< The action upon hitting the "new" button of
                                    the main menu.*/
  QSettings settings_;       /**< Holds a copy of the global settings of the
                                  application.*/
  QLabel *time_label_;       /**< The label holding info_string_.*/
  QHBoxLayout *time_layout_; /**< Holds time_label_ and lcd_.*/
  QString timer_string_;   /**< Holds a string of the time until the nex logging
                                event.*/
  QFrame *top_seperator_;  /**< A vertical line acting as a seperator.*/
  QLabel *version_label_;  /**< Contains the versioning number of the program.*/
  QStatusBar *status_bar_; /**< Status bar on the bottom of the window.*/
  QLabel *status_label_running_; /**< Indicator if the program is running.*/
  QLabel *status_label_time_;    /**< Holds the time until the next scheduled
                                      logging event.*/
  QProgressBar *status_progress_bar_; /**< A progress bar indicating the time
                                           until the next scheduled logging
                                           event.*/
  bool log_running_;      /**< True if the program is running, false if not.*/
  QToolBar *tool_bar_;    /**< The main tool bar*/
  QLabel *project_label_; /**< Holds the title of the project.*/
};
