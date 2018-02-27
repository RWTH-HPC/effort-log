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

#include "appenddialog.h"
#include "definitions.h"

#include <QDebug>

AppendDialog::AppendDialog(MainWindow *window, Activity *act) : QDialog() {
  main_window_ = window;
  project_ = window->project_;
  activity_ = act;
  Setup();
  CreateConnections();
  UpdateUI();
  setWindowFlags(Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
}

void AppendDialog::Setup() {
  // Activity count
  no_logged_activities_ = settings_.value("noLoggedActivities").toInt();

  // Specify logging interval
  QLabel *log_interval_label = new QLabel(tr("Time before executing this "
                                             "program:"));
  log_interval_spin_box_ = new QSpinBox(this);
  log_interval_spin_box_->setRange(0, 1000);
  log_interval_spin_box_->setSingleStep(1);
  log_interval_spin_box_->setValue(15);
  log_interval_spin_box_->setToolTip(tr("Set the time in minutes you want to "
                                        "be logged to the current project"));
  log_interval_spin_box_->setSuffix(" min");

  // Activity switcher (left part of the dialog)
  info_string_ = QString("What did you do the last %1 minutes?")
                     .arg(log_interval_spin_box_->value());
  group_activities_ = new QGroupBox(info_string_);
  activity_button_group_ = new QButtonGroup;
  QRadioButton *activity_switcher_buttons[NUM_ACTIVITIES];
  for (int i = 0; i < NUM_ACTIVITIES; i++) {
    activity_switcher_buttons[i] =
        new QRadioButton(activity_->kActivityType[i]);
    activity_button_group_->addButton(activity_switcher_buttons[i], i);
  }
  activity_switcher_buttons[0]->setShortcut(QKeySequence(Qt::Key_B));
  activity_switcher_buttons[0]->setToolTip(
      tr("Choose activity <font color='gray'>B</font>"));
  activity_switcher_buttons[1]->setShortcut(QKeySequence(Qt::Key_H));
  activity_switcher_buttons[1]->setToolTip(
      tr("Choose activity <font color='gray'>H</font>"));
  activity_switcher_buttons[2]->setShortcut(QKeySequence(Qt::Key_S));
  activity_switcher_buttons[2]->setToolTip(
      tr("Choose activity <font color='gray'>S</font>"));
  activity_switcher_buttons[3]->setShortcut(QKeySequence(Qt::Key_P));
  activity_switcher_buttons[3]->setToolTip(
      tr("Choose activity <font color='gray'>P</font>"));
  activity_switcher_buttons[4]->setShortcut(QKeySequence(Qt::Key_T));
  activity_switcher_buttons[4]->setToolTip(
      tr("Choose activity <font color='gray'>T</font>"));
  activity_switcher_buttons[5]->setShortcut(QKeySequence(Qt::Key_D));
  activity_switcher_buttons[5]->setToolTip(
      tr("Choose activity <font color='gray'>D</font>"));
  activity_switcher_buttons[6]->setShortcut(QKeySequence(Qt::Key_U));
  activity_switcher_buttons[6]->setToolTip(
      tr("Choose activity <font color='gray'>U</font>"));
  activity_switcher_buttons[7]->setShortcut(QKeySequence(Qt::Key_E));
  activity_switcher_buttons[7]->setToolTip(
      tr("Choose activity <font color='gray'>E</font>"));
  activity_switcher_buttons[8]->setShortcut(QKeySequence(Qt::Key_O));
  activity_switcher_buttons[8]->setToolTip(
      tr("Choose activity <font color='gray'>O</font>"));
  activity_switcher_buttons[0]->setChecked(true); // Set default button

  // Comment field (right part of the dialog)
  comment_box_ = new QPlainTextEdit;
  comment_box_->setPlaceholderText(tr("Comment..."));

  // Button to skip the event
  skip_button_ = new QPushButton(tr("Skip"));
  skip_button_->setToolTip(tr("Skip event <font color='gray'>Q</font>"));
  skip_button_->setCheckable(true);
  skip_button_->setAutoDefault(false);
  skip_button_->setShortcut(QKeySequence(Qt::Key_Q));

  // Button to finish the input
  finish_button_ = new QPushButton(tr("Save"));
  finish_button_->setToolTip(tr("Finish input <font color='gray'>F</font>"));
  finish_button_->setCheckable(true);
  finish_button_->setAutoDefault(false);
  finish_button_->setShortcut(QKeySequence(Qt::Key_F));

  // Layout
  QVBoxLayout *activity_layout = new QVBoxLayout;
  for (int i = 0; i < NUM_ACTIVITIES; i++) {
    activity_layout->addWidget(activity_switcher_buttons[i]);
  }
  activity_layout->addStretch(1);
  group_activities_->setLayout(activity_layout);

  QHBoxLayout *culomn_layout = new QHBoxLayout;
  culomn_layout->addWidget(group_activities_);
  culomn_layout->addWidget(comment_box_);

  QHBoxLayout *button_layout = new QHBoxLayout;
  button_layout->addWidget(skip_button_, 0, Qt::AlignCenter);
  button_layout->addWidget(finish_button_, 0, Qt::AlignCenter);

  QHBoxLayout *time_layout = new QHBoxLayout;
  time_layout->addWidget(log_interval_label);
  time_layout->addWidget(log_interval_spin_box_);

  main_layout_ = new QVBoxLayout;
  main_layout_->addLayout(time_layout);
  main_layout_->addLayout(culomn_layout);
  main_layout_->addLayout(button_layout);
  setLayout(main_layout_);

  QString title_string =
      QString("Appending a logging event to project: %1")
          .arg(settings_.value("conf/projectTitle").toString());
  group_activities_->setFocus();
  setWindowTitle(title_string);
}

void AppendDialog::CreateConnections() {
  connect(log_interval_spin_box_, SIGNAL(valueChanged(int)), this,
          SLOT(UpdateUI()));
  connect(skip_button_, SIGNAL(released()), this, SLOT(reject()));
  connect(finish_button_, SIGNAL(released()), this, SLOT(accept()));
}

void AppendDialog::UpdateUI() {
  info_string_ = QString("What did you do the last %1 minutes?")
                     .arg(log_interval_spin_box_->value());
  group_activities_->setTitle(info_string_);
  main_layout_->update();
}

void AppendDialog::accept() {
  no_logged_activities_ = settings_.value("noLoggedActivities").toInt() + 1;
  settings_.setValue("noLoggedActivities", no_logged_activities_);
  activity_->SetCurTime(QDateTime::currentDateTime());
  activity_->SetLastTime(settings_.value("lastLogTime").toDateTime());
  activity_->SetIntervalTime(log_interval_spin_box_->value());
  activity_->SetSavedEvents(no_logged_activities_);
  activity_->SetType(activity_button_group_->checkedButton()->text());
  settings_.setValue("lastLogTime", QTime::currentTime());
  activity_->SetProjectTitle(settings_.value("conf/projectTitle").toString());
  activity_->SetUserName(settings_.value("conf/userName").toString());
  activity_->SetLogInterval(settings_.value("conf/logInterval").toInt());
  activity_->SetMsComment(comment_box_->toPlainText());
  activity_->SetScheduler(2);

  if (VERBOSE) {
    qDebug() << "\nActivity type:" << activity_->GetType();
    qDebug() << "Comment:" << activity_->GetMsComment();
    qDebug() << "User name:" << activity_->GetUserName();
    qDebug() << "Project title:" << activity_->GetProjectTitle();
    qDebug() << "Current time:" << activity_->GetCurTime().toString();
    qDebug() << "Time of the last log:" << activity_->GetLastTime().toString();
    qDebug() << "Specified logging interval:" << activity_->GetLogInterval();
    qDebug() << "Actual logging interval:" << activity_->GetIntervalTime();
    qDebug() << "No of saved events:" << activity_->GetSavedEvents();
  }
  main_window_->SetupAnimation();
  settings_.setValue("conf/append", true);
  QDialog::accept();
}

void AppendDialog::reject() {
  QMessageBox::StandardButton button;
  button = QMessageBox::question(
      this, APP_NAME,
      tr("Are you sure you want to "
         "skip this addendum? No logging event will be "
         "added to the current project.\n"),
      QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes);
  if (button != QMessageBox::No) {
    settings_.setValue("conf/append", false);
    QDialog::reject();
  }
}
