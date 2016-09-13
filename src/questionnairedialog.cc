/*
 * Copyright © 2016 by IT Center, RWTH Aachen University
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

#include "questionnairedialog.h"
#include "definitions.h"
#include "milestonedialog.h"

#include <QDebug>
#include <QtMath>

QuestionnaireDialog::QuestionnaireDialog(MainWindow *window, int scheduler)
    : QDialog() {
  main_window_ = window;
  project_ = window->project_;
  scheduler_ = scheduler;
  Setup();
  CreateConnections();
  UpdateUI();
  setWindowFlags(Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
}

void QuestionnaireDialog::Setup() {
  QSettings settings;
  activity_ = new Activity;
  no_logged_activities_ = settings.value("noLoggedActivities").toInt();

  // Activity switcher (left part of the dialog)
  activity_label_ = new QLabel(info_string_);
  group_activities_ = new QGroupBox();
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
  QLabel *comment_label = new QLabel("Comment on this activity:");
  comment_label->setWordWrap(true);
  comment_box_ = new QPlainTextEdit;
  comment_box_->setPlaceholderText(tr("Comment..."));

  // Project milestones
  QLabel *m_label = new QLabel("Did you reach one of the following milestones "
                               "during the last logging interval?");
  m_label->setWordWrap(true);
  m_group_ = new QButtonGroup(this);
  m_group_->setExclusive(false);
  QCheckBox *checkbox1 = new QCheckBox(tr("Working serial version"));
  QCheckBox *checkbox2 = new QCheckBox(tr("Tuned serial version"));
  QCheckBox *checkbox3 = new QCheckBox(tr("Working parallel version"));
  QCheckBox *checkbox4 = new QCheckBox(tr("Tuned parallel version"));
  QCheckBox *checkbox5 = new QCheckBox(tr("Other"));
  m_group_->addButton(checkbox1);
  m_group_->addButton(checkbox2);
  m_group_->addButton(checkbox3);
  m_group_->addButton(checkbox4);
  m_group_->addButton(checkbox5);
  last_m_button_ = NULL;
  m_button_pressed_ = false;

  // Button to skip the event
  skip_button_ = new QPushButton(tr("Skip"));
  skip_button_->setToolTip(tr("Skip event <font color='gray'>Q</font>"));
  skip_button_->setCheckable(true);
  skip_button_->setAutoDefault(false);
  skip_button_->setShortcut(QKeySequence(Qt::Key_Q));

  // Button to finish the input
  finish_button_ = new QPushButton(tr("Finish"));
  finish_button_->setToolTip(tr("Finish input <font color='gray'>F</font>"));
  finish_button_->setCheckable(true);
  finish_button_->setAutoDefault(false);
  finish_button_->setShortcut(QKeySequence(Qt::Key_F));

  // Separator
  QFrame *vline1 = new QFrame();
  vline1->setGeometry(QRect(/* ... */));
  vline1->setFrameShape(QFrame::VLine);
  vline1->setFrameShadow(QFrame::Sunken);
  QFrame *vline2 = new QFrame();
  vline2->setGeometry(QRect(/* ... */));
  vline2->setFrameShape(QFrame::VLine);
  vline2->setFrameShadow(QFrame::Sunken);
  QFrame *botton_line = new QFrame();
  botton_line->setGeometry(QRect(/* ... */));
  botton_line->setFrameShape(QFrame::HLine);
  botton_line->setFrameShadow(QFrame::Sunken);

  // Layout
  QVBoxLayout *activity_layout = new QVBoxLayout;
  activity_layout->addWidget(activity_label_);
  for (int i = 0; i < NUM_ACTIVITIES; i++) {
    activity_layout->addWidget(activity_switcher_buttons[i]);
  }
  activity_layout->addStretch(1);

  QVBoxLayout *comment_layout = new QVBoxLayout;
  comment_layout->addWidget(comment_label);
  comment_layout->addWidget(comment_box_);

  QVBoxLayout *m_layout = new QVBoxLayout;
  m_layout->addWidget(m_label);
  m_layout->addWidget(checkbox1);
  m_layout->addWidget(checkbox2);
  m_layout->addWidget(checkbox3);
  m_layout->addWidget(checkbox4);
  m_layout->addWidget(checkbox5);
  m_layout->addStretch(1);

  QHBoxLayout *button_layout = new QHBoxLayout;
  button_layout->addWidget(skip_button_, 0, Qt::AlignCenter);
  button_layout->addWidget(finish_button_, 0, Qt::AlignCenter);

  QHBoxLayout *colomn_layout = new QHBoxLayout;
  colomn_layout->addLayout(activity_layout);
  colomn_layout->addWidget(vline1);
  colomn_layout->addLayout(comment_layout);
  colomn_layout->addWidget(vline2);
  colomn_layout->addLayout(m_layout);

  main_layout_ = new QVBoxLayout;
  main_layout_->addLayout(colomn_layout);
  main_layout_->addWidget(botton_line);
  main_layout_->addLayout(button_layout);
  setLayout(main_layout_);

  QString title_string =
      QString("Logging event #%1:   %2 - now")
          .arg(no_logged_activities_ + 1)
          .arg(settings.value("lastLogTime").toDateTime().toString("hh:mm ap"));
  group_activities_->setFocus();
  setWindowTitle(title_string);
  return;
}

void QuestionnaireDialog::CreateConnections() {
  QTimer *countdown_timer = new QTimer();
  connect(countdown_timer, SIGNAL(timeout()), this, SLOT(UpdateUI()));
  countdown_timer->start(60000);
  connect(skip_button_, SIGNAL(released()), this, SLOT(reject()));
  connect(finish_button_, SIGNAL(released()), this, SLOT(accept()));
  // connect(m_group_, SIGNAL(buttonClicked(QAbstractButton*)), this,
  //        SLOT(OnPressedMButton(QAbstractButton*)));
  return;
}

void QuestionnaireDialog::UpdateUI() {
  QSettings settings;
  int interval = QTime::currentTime().msecsSinceStartOfDay() -
                 settings.value("lastLogTime").toTime().msecsSinceStartOfDay();
  interval /= (1000 * 60);
  if (interval == 1) {
    info_string_ = QString("What did you do the last minute?");
  } else {
    info_string_ =
        QString("What did you do the last %1 minutes?").arg(interval);
  }
  activity_label_->setText(info_string_);
  activity_label_->update();

  if (VERBOSE) {
    qDebug() << "\n@Dialog: UpdateUI called. Time interval: " << interval;
    qDebug() << "Last log:" << settings.value("lastLogTime").toTime();
    qDebug() << "Interval since last log:" << interval;
  }
  return;
}

void QuestionnaireDialog::accept() {
  QSettings settings;
  no_logged_activities_ = settings.value("noLoggedActivities").toInt() + 1;
  settings.setValue("noLoggedActivities", no_logged_activities_);
  QDateTime cur_time = QDateTime::currentDateTime();
  activity_->SetCurTime(cur_time);
  activity_->SetLastTime(settings.value("lastLogTime").toDateTime());
  // activity_->SetIntervalTime(qCeil((
  //  activity_->GetCurTime().time().msecsSinceStartOfDay()
  //  - activity_->GetLastTime().time().msecsSinceStartOfDay()) / 60000.0));
  // activity_->SetIntervalTime(qCeil(cur_time.secsTo(activity_->GetLastTime()))/-60.0);
  activity_->SetIntervalTime(
      qCeil(cur_time.secsTo(activity_->GetLastTime()) / -60.0));
  activity_->SetSavedEvents(no_logged_activities_);
  activity_->SetType(activity_button_group_->checkedButton()->text());
  settings.setValue("lastLogTime", QDateTime::currentDateTime());
  activity_->SetProjectTitle(settings.value("conf/projectTitle").toString());
  activity_->SetUserName(settings.value("conf/userName").toString());
  activity_->SetLogInterval(settings.value("conf/logInterval").toInt());
  activity_->SetComment(comment_box_->toPlainText());
  switch (scheduler_) {
  case 0:
    activity_->SetScheduler(1);
    break;
  case 2:
    activity_->SetScheduler(3);
    break;
  case 3:
    activity_->SetScheduler(4);
    break;
  default:
    activity_->SetScheduler(0);
  }

  if (m_group_->checkedId() != -1) {
    QList<QAbstractButton *> buttons = m_group_->buttons();
    foreach (QAbstractButton *b, buttons) {
      if (b->isChecked() == true) {
        Milestone *m = new Milestone;
        if (project_->GetNoMilestones() > 0) {
          *m = project_->GetMilestone(project_->GetNoMilestones() - 1);
          m->SetMsId(-1);
        } else {
          m->SetMsId(0);
        }
        MilestoneDialog dialog(m, b->text());
        if (dialog.exec()) {
          m->SetTitle(b->text());
          m->SetTime(QDateTime::currentDateTime());
          m->SetEventId(project_->GetNoActivities());
          project_->AddMilestone(m);
        } else {
          free(m);
        }
      }
    }
  }

  if (VERBOSE) {
    qDebug() << "\nActivity type:" << activity_->GetType();
    qDebug() << "Comment:" << activity_->GetComment();
    qDebug() << "User name:" << activity_->GetUserName();
    qDebug() << "Project title:" << activity_->GetProjectTitle();
    qDebug() << "Current time:" << activity_->GetCurTime().toString();
    qDebug() << "Time of the last log:" << activity_->GetLastTime().toString();
    qDebug() << "Specified logging interval:" << activity_->GetLogInterval();
    qDebug() << "Actual logging interval:" << activity_->GetIntervalTime();
    qDebug() << "No of saved events:" << activity_->GetSavedEvents();
  }
  project_->AddActivity(*activity_);
  project_->StoreLog(settings.value("conf/logFile").toString());
  main_window_->SetupAnimation();
  QDialog::accept();
  return;
}

void QuestionnaireDialog::reject() {
  QMessageBox::StandardButton button;
  button = QMessageBox::question(this, APP_NAME, tr("Are you sure you want to "
                                                    "skip this log?\n"),
                                 QMessageBox::No | QMessageBox::Yes,
                                 QMessageBox::Yes);
  if (button != QMessageBox::No) {
    QSettings settings;
    settings.setValue("lastLogTime", QDateTime::currentDateTime());
    main_window_->SetupAnimation();
    QDialog::reject();
  }
  return;
}

void QuestionnaireDialog::OnPressedMButton(QAbstractButton *btn) {
  QList<QAbstractButton *> buttons = m_group_->buttons();
  foreach (QAbstractButton *b, buttons) { b->setChecked(false); }
  if (btn != last_m_button_ || m_button_pressed_ == false) {
    btn->setChecked(true);
    m_button_pressed_ = true;
  } else {
    m_button_pressed_ = false;
  }
  last_m_button_ = btn;
  return;
}
