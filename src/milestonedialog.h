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

#include <QComboBox>
#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QString>
#include <QVBoxLayout>

#include "milestone.h"

//! Class to create a new milestone
/*!
 * This class provides a dialog which lets users set up a new milestone.
 */
class MilestoneDialog : public QDialog {
  Q_OBJECT
public:
  //! The main constructor.
  /*!
   * \param[in] m A pointer to the milestone to be created
   */
  MilestoneDialog(Milestone *m, QString msg);
  //! Overrides the default accept() member.
  /*!
   * accept() is executed on successfully quitting the dialog.
   */
  void accept() Q_DECL_OVERRIDE;
  //! Overrides the default reject() member.
  /*!
   * reject() is executed on quitting the dialog before finishing the input of
   * all necessary fields.
   */
  void reject() Q_DECL_OVERRIDE;

private slots:
  //! Handles changes to the performance metrics
  /*!
   * \param[in] i The id of the active metric.
   */
  void PerfInputChanged(int i);
  //! Handles changes to the threads, nodes, etc. metrics
  /*!
   * \param[in] i The id of the active metric.
   */
  void ThreadsInputChanged(int i);
  //! Checks the user input
  /*!
   * The user can only click on OK when all required input fields are set
   * correctly.
   */
  void CheckInput();

private:
  //! Sets up the view of the dialog
  /*!
   * This method initilaizes all objects needed for visualization of the dialog.
   * All objects (widgets, layouts, etc.) are bind to main_layout_.
   * \see MilestoneDialog::main_layout_
   */
  void Setup();
  //! Takes care of all connections
  /*!
   * This method sets up the needed connections of the dialog. Theser are:
   *  - perf_box_ -> PerfInputChanged(int): execute PerfInputChanged(int) if
   * the active metric of perf_box_ is changed
   *  - threads_box_ -> ThreadsInputChanged(int): execute
   * Execute CheckInput() on changes to the following objects:
   *    - perf_box_
   *    - perf_comment_
   *    - arc_comment_
   *    - threads_box_
   *    - threads_comment_
   *    - compiler_comment_
   *    - model_comment_
   *  - finish_button_ -> accept(): execute accept() if accepted() is
   * pressed
   *  - skip_button_ -> reject(): execute reject() if rejected() is pressed
   */
  void CreateConnections();
  //! Loads values from previous milestones
  /*!
   * This method sets the dialog to keys used in the previous milestones (if
   * existent). If this is the first milestone of a project this method does
   * nothing.
   */
  void LoadPreviousMS();

  Milestone *ms_; /**< A pointer to the associate milestone \see Milestone.*/
  QPushButton *finish_button_; /**< Button to exit the dialog after finishing
                                    all mandatory input.*/
  QVBoxLayout *main_layout_;   /**< The main layout of the dialog.*/
  QPushButton *skip_button_;  /**< Button to exit the dialog without logging the
                                   event to the log file (skipping this event)*/
  QLineEdit *perf_comment_;   /**< Lets user specify a comment about the
                                   performance.*/
  QDoubleSpinBox *perf_spin_; /**< Lets user specify the performance.*/
  QSpinBox *threads_spin_;    /**< Lets user specify count of nodes, threads,
                                   etc.*/
  QString msg_;               /**< Specifies the used milestone.*/
  QComboBox *perf_box_;       /**< Lets user specify the performance metric.*/
  QComboBox *threads_box_;    /**< Lets user specify the metric for counting
                                   nodes, threads, etc.*/
  QLineEdit *arc_comment_;    /**< Lets user specify the architecture.*/
  QLineEdit *threads_comment_;  /**< Lets user specify a comment about the
                                  count of nodes, threads, etc.*/
  QLineEdit *compiler_comment_; /**< Lets user specify the used compiler.*/
  QLineEdit *model_comment_;    /**< Lets user specify the used programming
                                     model.*/
  QLineEdit *comment_;          /**< Lets user specify a general comment about
                                     the milestone.*/
  QLineEdit *data_size_;        /**< Lets user specify the data size of the
                                             milestone.*/
};
