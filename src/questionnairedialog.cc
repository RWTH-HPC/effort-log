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

#include "questionnairedialog.h"
#include "definitions.h"
#include "logview.h"
#include "milestonedialog.h"

#include <QDebug>
#include <QDir>
#include <QtMath>

QuestionnaireDialog::QuestionnaireDialog(MainWindow *window, int scheduler)
    : QDialog() {
  main_window_ = window;
  project_ = window->project_;
  scheduler_ = scheduler;
  Setup();
  CreateConnections();
  UpdateUI();
  setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
}

void QuestionnaireDialog::Setup() {
  QSettings settings;
  activity_ = new Activity;
  no_logged_activities_ = settings.value("noLoggedActivities").toInt();

  // Activity form
  act_form_ = new QstForm();
  act_form_->SetTitle(info_string_);
  act_form_->SetToolTip(
      tr("Describe how you were mainly working since the last diary entry"));
  act_form_->SetLine(false);

  // Performance
  perf_group_ = new QButtonGroup(this);
  perf_group_->setExclusive(true);
  QCheckBox *perf_checkbox1 = new QCheckBox(tr("Yes"));
  QCheckBox *perf_checkbox2 = new QCheckBox(tr("No"));
  perf_group_->addButton(perf_checkbox1);
  perf_group_->addButton(perf_checkbox2);
  perf_group_->setId(perf_checkbox1, 1);
  perf_group_->setId(perf_checkbox2, 2);
  perf_checkbox2->setChecked(true);

  QLabel *perf_section_label_ =
      new QLabel(tr("<font color='red'>*</font> 2. Did you measure the "
                    "performance of your application?"));
  perf_section_label_->setWordWrap(true);
  QLabel *perf_label =
      new QLabel(tr("<font color='red'>*</font> Which performance does your "
                    "application currently reach?"));
  perf_label->setWordWrap(true);
  perf_box_ = new QComboBox;
  perf_box_->addItem(tr("Execution time in seconds"));
  perf_box_->addItem(tr("Execution time in minutes"));
  perf_box_->addItem(tr("Execution time in hours"));
  perf_box_->addItem(tr("Throughput in GFlop/s"));
  perf_box_->addItem(tr("Speedup"));
  perf_box_->addItem(tr("Other"));
  perf_spin_ = new QDoubleSpinBox;
  perf_spin_->setRange(0.0, 10000.0);
  perf_comment_ = new QLineEdit;
  perf_comment_->setPlaceholderText(tr("Specify performance"));
  perf_comment_->hide();
  perf_box_->setCurrentIndex(0);
  perf_spin_->setSuffix(tr(" sec"));

  // No threads, nodes, etc.
  QLabel *threads_label =
      new QLabel(tr("<font color='red'>*</font> No threads, nodes, etc. used "
                    "during the performance measurements"));
  threads_label->setWordWrap(true);
  threads_box_ = new QComboBox;
  threads_box_->addItem(tr("Number of threads"));
  threads_box_->addItem(tr("Number of nodes"));
  threads_box_->addItem(tr("Other"));
  threads_spin_ = new QSpinBox;
  threads_spin_->setRange(0, 10000000);
  threads_spin_->setSuffix(tr(" threads"));
  threads_box_->setCurrentIndex(0);
  threads_comment_ = new QLineEdit;
  threads_comment_->setToolTip(tr("Information on the number of threads, "
                                  "nodes, etc. used for the performance "
                                  "measurements"));
  threads_comment_->setPlaceholderText(tr("No threads, nodes, etc."));
  threads_comment_->hide();

  // Architecture
  arc_form_ = new QstForm();
  arc_form_->SetTitle(tr("Architecture"));
  arc_form_->SetToolTip(tr(
      "Information on the architecture used for the performance measurements"));
  arc_form_->SetPlaceholderText(tr("E.g. Intel Xeon X7550"));
  arc_form_->SetLine(false);

  // Compiler
  compiler_form_ = new QstForm();
  compiler_form_->SetTitle(tr("Compiler"));
  compiler_form_->SetToolTip(tr("Information on used compiler used for the "
                                "performance measurements"));
  compiler_form_->SetPlaceholderText(tr("E.g. GCC 7.1"));
  compiler_form_->SetLine(false);

  // Programming model
  model_form_ = new QstForm();
  model_form_->SetTitle(tr("Programming model"));
  model_form_->SetToolTip(
      tr("Information on the programming model used for the "
         "performance measurements"));
  model_form_->SetPlaceholderText(tr("E.g. OpenMP"));
  model_form_->SetLine(false);

  // Data size
  data_form_ = new QstForm();
  data_form_->SetTitle(tr("Data size"));
  data_form_->SetToolTip(tr("Information on the size of the used data set"));
  data_form_->SetPlaceholderText(tr("E.g. benchmark xyz with large data set"));
  data_form_->SetLine(false);

  // Comment field for the activity
  QLabel *comment_label = new QLabel("3. Comment on this activity:");
  comment_label->setWordWrap(true);
  comment_box_ = new QPlainTextEdit;
  comment_box_->setPlaceholderText(tr("Comment..."));
  comment_box_->setMaximumHeight(45);
  comment_box_->setToolTip(
      tr("Provide additional information of your last development activity"));

  // Project milestones
  QLabel *ms_label = new QLabel("<font color='red'>*</font> 4. Did you reach a "
                                "milestone during the last logging interval?");
  ms_label->setWordWrap(true);
  ms_label->setToolTip(tr("Select yes if you reached a milestone during the "
                          "last logging interval.<br>"
                          "Additional questions will then appear."));

  ms_group_ = new QButtonGroup(this);
  ms_group_->setExclusive(true);
  QCheckBox *ms_checkbox1 = new QCheckBox(tr("Yes"));
  QCheckBox *ms_checkbox2 = new QCheckBox(tr("No"));
  ms_group_->addButton(ms_checkbox1);
  ms_group_->addButton(ms_checkbox2);
  ms_group_->setId(ms_checkbox1, 1);
  ms_group_->setId(ms_checkbox2, 2);
  ms_checkbox2->setChecked(true);

  ms_title_label_ =
      new QLabel("<font color='red'>*</font> Title of the milestone:");
  ms_line_edit_ = new QLineEdit;
  ms_comment_label_ = new QLabel(tr("11. Comment on the milestone:"));
  ms_comment_label_->setWordWrap(true);
  ms_comment_ = new QPlainTextEdit;
  ms_comment_->setToolTip(
      tr("Provide additional information for this milestone"));
  ms_comment_->setPlaceholderText(tr("Comment..."));
  ms_comment_->setMaximumHeight(45);
  ms_title_label_->hide();
  ms_line_edit_->hide();
  ms_comment_label_->hide();
  ms_comment_->hide();

  // Button to skip the event
  skip_button_ = new QPushButton(tr("Skip"));
  skip_button_->setToolTip(tr("Skip event <font color='gray'>Q</font>"));
  skip_button_->setCheckable(true);
  skip_button_->setAutoDefault(false);
  skip_button_->setShortcut(QKeySequence(Qt::Key_Q));

  // Button to read the log
  log_button_ = new QPushButton(tr("Read Log"));
  log_button_->setToolTip(
      tr("Read current log file <font color='gray'>Q</font>"));
  log_button_->setCheckable(true);
  log_button_->setAutoDefault(false);
  log_button_->setShortcut(QKeySequence(Qt::Key_L));

  // Button to finish the input
  finish_button_ = new QPushButton(tr("Finish"));
  finish_button_->setToolTip(tr("Finish input <font color='gray'>F</font>"));
  finish_button_->setCheckable(true);
  finish_button_->setAutoDefault(false);
  finish_button_->setShortcut(QKeySequence(Qt::Key_F));
  finish_button_->setEnabled(false);

  // Separator
  QFrame *botton_line = new QFrame();
  botton_line->setGeometry(QRect(/* ... */));
  botton_line->setFrameShape(QFrame::HLine);
  botton_line->setFrameShadow(QFrame::Sunken);

  // Layout
  perf_buttons_ = new QHBoxLayout;
  perf_buttons_->addWidget(perf_checkbox1);
  perf_buttons_->addWidget(perf_checkbox2);

  QHBoxLayout *perf_layout = new QHBoxLayout;
  perf_layout->addWidget(perf_box_);
  perf_layout->addWidget(perf_spin_);
  perf_layout->addWidget(perf_comment_);

  QHBoxLayout *threads_layout = new QHBoxLayout;
  threads_layout->addWidget(threads_box_);
  threads_layout->addWidget(threads_spin_);
  threads_layout->addWidget(threads_comment_);

  ms_buttons_ = new QHBoxLayout;
  ms_buttons_->addWidget(ms_checkbox1);
  ms_buttons_->addWidget(ms_checkbox2);

  QHBoxLayout *button_layout = new QHBoxLayout;
  button_layout->addWidget(skip_button_, 0, Qt::AlignCenter);
  button_layout->addWidget(log_button_, 0, Qt::AlignCenter);
  button_layout->addWidget(finish_button_, 0, Qt::AlignCenter);

  QVBoxLayout *act_layout = new QVBoxLayout;
  act_layout->addWidget(act_form_);
  QGroupBox *act_group = new QGroupBox;
  act_group->setTitle(tr("Activity"));
  act_group->setLayout(act_layout);
  act_group->setToolTip(
      tr("Describe how you were mainly working since the last diary entry"));

  QVBoxLayout *perf_detailed_layout = new QVBoxLayout;
  perf_detailed_layout->addWidget(perf_label);
  perf_detailed_layout->addLayout(perf_layout);
  perf_detailed_layout->addWidget(threads_label);
  perf_detailed_layout->addLayout(threads_layout);
  perf_detailed_layout->addWidget(arc_form_);
  perf_detailed_layout->addWidget(compiler_form_);
  perf_detailed_layout->addWidget(model_form_);
  perf_detailed_layout->addWidget(data_form_);

  perf_widget_ = new QWidget;
  perf_widget_->setLayout(perf_detailed_layout);
  perf_widget_->hide();

  QVBoxLayout *perf_group_layout = new QVBoxLayout;
  perf_group_layout->addWidget(perf_section_label_);
  perf_group_layout->addLayout(perf_buttons_);
  perf_group_layout->addWidget(perf_widget_);
  QGroupBox *perf_group = new QGroupBox;
  perf_group->setTitle(tr("Performance"));
  perf_group->setLayout(perf_group_layout);
  perf_group->setToolTip(tr("Leave at default if you did not measure the "
                            "performance of your application."));

  QVBoxLayout *comment_layout = new QVBoxLayout;
  comment_layout->addWidget(comment_label);
  comment_layout->addWidget(comment_box_);
  QGroupBox *comment_group = new QGroupBox;
  comment_group->setTitle(tr("Comment"));
  comment_group->setLayout(comment_layout);
  comment_group->setMinimumHeight(120);
  comment_group->setMaximumHeight(120);
  comment_group->setToolTip(
      tr("Provide additional information of your last development activity"));

  QVBoxLayout *ms_layout = new QVBoxLayout;
  ms_layout->addWidget(ms_label);
  ms_layout->addLayout(ms_buttons_);
  ms_layout->addWidget(ms_title_label_);
  ms_layout->addWidget(ms_line_edit_);
  ms_layout->addWidget(ms_comment_label_);
  ms_layout->addWidget(ms_comment_);
  QGroupBox *ms_group = new QGroupBox;
  ms_group->setTitle(tr("Milestone"));
  ms_group->setLayout(ms_layout);
  ms_group->setToolTip(tr("Select yes if you reached a milestone during the "
                          "last logging interval.<br>"
                          "Additional questions will then appear."));

  main_layout_ = new QVBoxLayout;
  main_layout_->addWidget(act_group);
  main_layout_->addWidget(perf_group);
  main_layout_->addWidget(comment_group);
  main_layout_->addWidget(ms_group);
  main_layout_->addWidget(botton_line);
  main_layout_->addLayout(button_layout);
  setLayout(main_layout_);

  QString title_string =
      QString("Logging event #%1:   %2 - now")
          .arg(no_logged_activities_ + 1)
          .arg(settings.value("lastLogTime").toDateTime().toString("hh:mm ap"));
  this->setWindowTitle(title_string);
  this->setWindowFlags(Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
  return;
}

void QuestionnaireDialog::CreateConnections() {
  QTimer *countdown_timer = new QTimer();
  connect(countdown_timer, SIGNAL(timeout()), this, SLOT(UpdateUI()));
  countdown_timer->start(60000);

  connect(perf_box_, SIGNAL(activated(int)), this, SLOT(PerfInputChanged(int)));
  connect(threads_box_, SIGNAL(activated(int)), this,
          SLOT(ThreadsInputChanged(int)));
  connect(perf_group_, SIGNAL(buttonClicked(int)), this,
          SLOT(PerfInputChanged()));
  connect(ms_group_, SIGNAL(buttonClicked(int)), this, SLOT(MsInputChanged()));

  connect(perf_group_, SIGNAL(buttonClicked(int)), this, SLOT(CheckInput()));
  connect(ms_group_, SIGNAL(buttonClicked(int)), this, SLOT(CheckInput()));
  connect(act_form_->line_edit_, SIGNAL(textChanged(QString)), this,
          SLOT(CheckInput()));
  connect(perf_box_, SIGNAL(currentIndexChanged(int)), this,
          SLOT(CheckInput()));
  connect(perf_comment_, SIGNAL(textChanged(QString)), this,
          SLOT(CheckInput()));
  connect(perf_spin_, SIGNAL(valueChanged(double)), this, SLOT(CheckInput()));
  connect(threads_box_, SIGNAL(currentIndexChanged(int)), this,
          SLOT(CheckInput()));
  connect(threads_comment_, SIGNAL(textChanged(QString)), this,
          SLOT(CheckInput()));
  connect(threads_spin_, SIGNAL(valueChanged(int)), this, SLOT(CheckInput()));
  connect(ms_line_edit_, SIGNAL(textChanged(QString)), this,
          SLOT(CheckInput()));

  connect(skip_button_, SIGNAL(released()), this, SLOT(reject()));
  connect(log_button_, SIGNAL(released()), this, SLOT(LogViewer()));
  connect(finish_button_, SIGNAL(released()), this, SLOT(accept()));
  return;
}

void QuestionnaireDialog::UpdateUI() {
  QSettings settings;
  int interval = QTime::currentTime().msecsSinceStartOfDay() -
                 settings.value("lastLogTime").toTime().msecsSinceStartOfDay();
  interval /= (1000 * 60);
  if (interval == 1) {
    info_string_ = QString("<font color='red'>*</font> 1. What were you "
                           "working on the last minute?");
  } else {
    info_string_ = QString("<font color='red'>*</font> 1. What were you "
                           "working on the last %1 minutes?")
                       .arg(interval);
  }
  act_form_->SetTitle(info_string_);

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
  activity_->SetIntervalTime(
      qCeil(cur_time.secsTo(activity_->GetLastTime()) / -60.0));
  activity_->SetSavedEvents(no_logged_activities_);
  activity_->SetType(act_form_->GetText());
  settings.setValue("lastLogTime", QDateTime::currentDateTime());
  activity_->SetProjectTitle(settings.value("conf/projectTitle").toString());
  activity_->SetUserName(settings.value("conf/userName").toString());
  activity_->SetLogInterval(settings.value("conf/logInterval").toInt());

  if (perf_group_->checkedId() == 1) {
    activity_->SetArc(arc_form_->GetText());
    activity_->SetModel(model_form_->GetText());
    activity_->SetPerfMetric(perf_box_->currentText());
    if (perf_box_->currentIndex() == 5) {
      activity_->SetPerfComment(perf_comment_->text());
    } else {
      activity_->SetPerfComment(perf_spin_->text());
    }
    activity_->SetCompiler(compiler_form_->GetText());
    activity_->SetThreadsType(threads_box_->currentText());
    if (threads_box_->currentIndex() == 2) {
      activity_->SetThreadsComment(threads_comment_->text());
    } else {
      activity_->SetThreadsComment(threads_spin_->text());
    }
    activity_->SetDataSize(data_form_->GetText());
  }

  if (ms_group_->checkedId() == 1) {
    activity_->SetMsTitle(ms_line_edit_->text());
    activity_->SetMsComment(comment_box_->toPlainText());
    activity_->SetMsId(project_->GetNoMilestones());
  } else {
    activity_->SetMsId(-1);
  }
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
  activity_->SetId(project_->GetNoActivities());
  project_->AddActivity(*activity_);
  project_->StoreLog(settings.value("conf/logFile").toString());
  main_window_->SetupAnimation();
  main_window_->SetQstRunning(false);
  QDialog::accept();
  return;
}

void QuestionnaireDialog::reject() {
  QMessageBox::StandardButton button;
  button = QMessageBox::question(this, APP_NAME,
                                 tr("Are you sure you want to "
                                    "skip this log?\n"),
                                 QMessageBox::No | QMessageBox::Yes,
                                 QMessageBox::Yes);
  if (button != QMessageBox::No) {
    QSettings settings;
    settings.setValue("lastLogTime", QDateTime::currentDateTime());
    main_window_->SetupAnimation();
    QDialog::reject();
  }
  main_window_->SetQstRunning(false);
  return;
}

void QuestionnaireDialog::PerfInputChanged(int i) {
  switch (i) {
  case 0:
    perf_spin_->setSuffix(tr(" sec"));
    perf_spin_->show();
    perf_comment_->hide();
    perf_spin_->setFocus();
    break;
  case 1:
    perf_spin_->setSuffix(tr(" min"));
    perf_spin_->show();
    perf_comment_->hide();
    perf_spin_->setFocus();
    break;
  case 2:
    perf_spin_->setSuffix(tr(" hour"));
    perf_spin_->show();
    perf_comment_->hide();
    perf_spin_->setFocus();
    break;
  case 3:
    perf_spin_->setSuffix(tr(" GFlop/s"));
    perf_spin_->show();
    perf_comment_->hide();
    perf_spin_->setFocus();
    break;
  case 4:
    perf_spin_->setSuffix(tr(""));
    perf_spin_->show();
    perf_comment_->hide();
    perf_spin_->setFocus();
    break;
  case 5:
    perf_spin_->hide();
    perf_comment_->show();
    perf_comment_->setFocus();
    break;
  }
  main_layout_->update();
  this->repaint();
  return;
}

void QuestionnaireDialog::ThreadsInputChanged(int i) {
  switch (i) {
  case 0:
    threads_spin_->setSuffix(tr(" threads"));
    threads_spin_->show();
    threads_comment_->hide();
    threads_spin_->setFocus();
    break;
  case 1:
    threads_spin_->setSuffix(tr(" nodes"));
    threads_spin_->show();
    threads_comment_->hide();
    threads_spin_->setFocus();
    break;
  case 2:
    threads_spin_->hide();
    threads_comment_->show();
    threads_comment_->setFocus();
    break;
  }
  main_layout_->update();
  this->repaint();
  return;
}

void QuestionnaireDialog::PerfInputChanged() {
  if (perf_group_->checkedId() == 1) {
    perf_widget_->show();
  } else if (perf_group_->checkedId() == 2) {
    perf_widget_->hide();
  }

  main_layout_->update();
  this->repaint();
  return;
}

void QuestionnaireDialog::MsInputChanged() {
  if (ms_group_->checkedId() == 1) {
    ms_title_label_->show();
    ms_line_edit_->show();
    ms_comment_label_->show();
    ms_comment_->show();
    ms_title_label_->setFocus();
  } else if (ms_group_->checkedId() == 2) {
    ms_title_label_->hide();
    ms_line_edit_->hide();
    ms_comment_label_->hide();
    ms_comment_->hide();
  }

  main_layout_->update();
  this->repaint();
  return;
}

void QuestionnaireDialog::CheckInput() {
  finish_button_->setEnabled(false);
  if (act_form_->GetText().isEmpty()) {
    return;
  }
  if (perf_group_->checkedId() == 1) {
    if (perf_box_->currentIndex() == 5) {
      if (perf_comment_->text().isEmpty()) {
        return;
      }
    } else {
      if (perf_spin_->value() <= 0.0) {
        return;
      }
    }
    if (threads_box_->currentIndex() == 2) {
      if (threads_comment_->text().isEmpty()) {
        return;
      }
    } else {
      if (threads_spin_->value() <= 0) {
        return;
      }
    }
  }
  if (ms_group_->checkedId() == 1) {
    if (ms_line_edit_->text().isEmpty()) {
      return;
    }
  }
  finish_button_->setEnabled(true);
  return;
}

void QuestionnaireDialog::OnPressedMButton(QAbstractButton *btn) {
  QList<QAbstractButton *> buttons = ms_group_->buttons();
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

void QuestionnaireDialog::ReadLog() {
  QSettings settings;
  project_->ReadLog(
      QDir::toNativeSeparators(settings.value("conf/logFile").toString()));
  return;
}

void QuestionnaireDialog::LogViewer() {
  LogView view(project_);
  view.resize(450, 300);
  view.exec();
  return;
}
