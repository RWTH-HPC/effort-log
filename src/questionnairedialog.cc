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

#include <QDebug>
#include <QDir>
#include <QScrollBar>
#include <QtMath>

QuestionnaireDialog::QuestionnaireDialog(MainWindow *window, int scheduler)
    : QDialog() {
  main_window_ = window;
  project_ = window->project_;
  scheduler_ = scheduler;
  Setup();
  CreateConnections();
  UpdateUI();
}

void QuestionnaireDialog::Setup() {
  QSettings settings;
  activity_ = new Activity;
  no_logged_activities_ = settings.value("noLoggedActivities").toInt();

  // Specify logging interval
  QLabel *log_interval_label = new QLabel(
      tr("How long have you been working before executing EffortLog?"));
  if (scheduler_ == 1) {
    log_interval_spin_box_ = new QSpinBox(this);
    log_interval_spin_box_->setRange(0, 1000);
    log_interval_spin_box_->setSingleStep(1);
    log_interval_spin_box_->setValue(15);
    log_interval_spin_box_->setToolTip(tr("Set the time in minutes you want to "
                                          "be logged to the current project"));
    log_interval_spin_box_->setSuffix(" min");
  }
  // Activity form
  act_form_ = new QstForm();
  act_form_->SetTitle(info_string_);
  act_form_->SetToolTip(tr("Describe how you were mainly working since the "
                           "last diary entry. Typical choices include:"
                           "<br> - Break"
                           "<br> - Thinking"
                           "<br> - Serial"
                           "<br> - Parallelizing"
                           "<br> - Testing"
                           "<br> - Debugging"
                           "<br> - Tuning"
                           "<br> - Experimenting"));
  act_form_->SetLine(false);
  if (scheduler_ == 1)
    act_form_->SetCompleter(activity_->kActivityType);
  else
    act_form_->SetCompleter(project_->GetUniqueActivities());

  // Performance
  perf_group_ = new QButtonGroup(this);
  perf_group_->setExclusive(true);
  perf_checkbox1_ = new QCheckBox(tr("Yes"));
  QCheckBox *perf_checkbox2 = new QCheckBox(tr("No"));
  perf_group_->addButton(perf_checkbox1_);
  perf_group_->addButton(perf_checkbox2);
  perf_group_->setId(perf_checkbox1_, 1);
  perf_group_->setId(perf_checkbox2, 2);
  perf_checkbox1_->setChecked(true);
  perf_checkbox1_->setFixedSize(perf_checkbox1_->sizeHint());
  perf_checkbox2->setFixedSize(perf_checkbox2->sizeHint());

  perf_section_label_ =
      new QLabel(tr("<font color='red'>*</font> 2. Did you measure the "
                    "performance of your application?"));
  perf_section_label_->setWordWrap(true);
  perf_section_label_->setFixedHeight(
      perf_section_label_->contentsMargins().top() +
      perf_section_label_->contentsMargins().bottom() +
      2 * fontMetrics().lineSpacing());
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
  if (scheduler_ != 1) {
    arc_form_->SetCompleter(project_->GetUniqueArchitectures());
    arc_form_->SetText(project_->GetLastPerf().GetArc());
  }

  // Compiler
  compiler_form_ = new QstForm();
  compiler_form_->SetTitle(tr("Compiler"));
  compiler_form_->SetToolTip(tr("Information on used compiler used for the "
                                "performance measurements"));
  compiler_form_->SetPlaceholderText(tr("E.g. GCC 7.1"));
  compiler_form_->SetLine(false);
  if (scheduler_ != 1) {
    compiler_form_->SetCompleter(project_->GetUniqueCompilers());
    compiler_form_->SetText(project_->GetLastPerf().GetCompiler());
  }

  // Programming model
  model_form_ = new QstForm();
  model_form_->SetTitle(tr("Programming model"));
  model_form_->SetToolTip(
      tr("Information on the programming model used for the "
         "performance measurements"));
  model_form_->SetPlaceholderText(tr("E.g. OpenMP"));
  model_form_->SetLine(false);
  if (scheduler_ != 1) {
    model_form_->SetCompleter(project_->GetUniqueProgModels());
    model_form_->SetText(project_->GetLastPerf().GetModel());
  }

  // Data size
  data_form_ = new QstForm();
  data_form_->SetTitle(tr("Data size"));
  data_form_->SetToolTip(tr("Information on the size of the used data set"));
  data_form_->SetPlaceholderText(tr("E.g. benchmark xyz with large data set"));
  data_form_->SetLine(false);
  if (scheduler_ != 1) {
    data_form_->SetCompleter(project_->GetUniqueDataSizes());
    data_form_->SetText(project_->GetLastPerf().GetDataSize());
  }

  // Comment field for the activity
  QLabel *comment_label = new QLabel("3. Comment on this activity:");
  comment_label->setWordWrap(true);
  comment_label->setFixedHeight(comment_label->contentsMargins().top() +
                                comment_label->contentsMargins().bottom() +
                                fontMetrics().lineSpacing());
  comment_box_ = new QPlainTextEdit;
  comment_box_->setPlaceholderText(tr("Comment..."));
  comment_box_->setToolTip(
      tr("Provide additional information of your last development activity"));
  comment_box_->setFixedHeight(comment_box_->contentsMargins().top() +
                               comment_box_->contentsMargins().bottom() +
                               2.5 * fontMetrics().lineSpacing());

  // Project milestones
  ms_label_ = new QLabel("<font color='red'>*</font> 4. Did you reach a "
                         "milestone during the last logging interval?");
  ms_label_->setWordWrap(true);
  ms_label_->setToolTip(tr("Select yes if you reached a milestone during the "
                           "last logging interval.<br>"
                           "Additional questions will then appear."));
  ms_label_->setFixedHeight(ms_label_->contentsMargins().top() +
                            ms_label_->contentsMargins().bottom() +
                            2 * fontMetrics().lineSpacing());

  ms_group_ = new QButtonGroup(this);
  ms_group_->setExclusive(true);
  ms_checkbox1_ = new QCheckBox(tr("Yes"));
  QCheckBox *ms_checkbox2 = new QCheckBox(tr("No"));
  ms_group_->addButton(ms_checkbox1_);
  ms_group_->addButton(ms_checkbox2);
  ms_group_->setId(ms_checkbox1_, 1);
  ms_group_->setId(ms_checkbox2, 2);
  ms_checkbox2->setChecked(true);
  ms_checkbox1_->setFixedSize(ms_checkbox1_->sizeHint());
  ms_checkbox2->setFixedSize(ms_checkbox2->sizeHint());

  ms_form_ = new QstForm();
  ms_form_->SetTitle(tr("<font color='red'>*</font> Title of the milestone:"));
  ms_form_->SetToolTip(tr("The name of the milestone. Typical choices include:"
                          "<br> - Working serial version"
                          "<br> - Profiled serial version"
                          "<br> - Working parallel version"
                          "<br> - Profiled parallel version"
                          "<br> - Tuned parallel version"));
  ms_form_->SetPlaceholderText(tr("E.g. working serial version"));
  ms_form_->SetLine(false);
  if (scheduler_ == 1)
    ms_form_->SetCompleter(activity_->kMilestoneType);
  else
    ms_form_->SetCompleter(project_->GetUniqueMs());
  ms_form_->hide();

  ms_comment_label_ = new QLabel(tr("11. Comment on the milestone:"));
  ms_comment_label_->setWordWrap(true);
  ms_comment_label_->setFixedHeight(
      ms_comment_label_->contentsMargins().top() +
      ms_comment_label_->contentsMargins().bottom() +
      fontMetrics().lineSpacing());
  ms_comment_ = new QPlainTextEdit;
  ms_comment_->setToolTip(
      tr("Provide additional information for this milestone"));
  ms_comment_->setPlaceholderText(tr("Comment..."));
  ms_comment_->setFixedHeight(ms_comment_->contentsMargins().top() +
                              ms_comment_->contentsMargins().bottom() +
                              2.5 * fontMetrics().lineSpacing());
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
  if (scheduler_ == 1) {
    log_button_->setEnabled(false);
    log_button_->setToolTip(
        tr("You need to set up the current project to read it's log files"));
  }

  // Button to finish the input
  finish_button_ = new QPushButton(tr("Finish"));
  finish_button_->setToolTip(tr("Finish input <font color='gray'>F</font>"));
  finish_button_->setCheckable(true);
  finish_button_->setAutoDefault(false);
  finish_button_->setShortcut(QKeySequence(Qt::Key_F));
  finish_button_->setEnabled(false);

  // Layout
  perf_buttons_ = new QHBoxLayout;
  perf_buttons_->addWidget(perf_checkbox1_);
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
  ms_buttons_->addWidget(ms_checkbox1_);
  ms_buttons_->addWidget(ms_checkbox2);

  QHBoxLayout *button_layout = new QHBoxLayout;
  button_layout->addWidget(skip_button_, 0, Qt::AlignCenter);
  button_layout->addWidget(log_button_, 0, Qt::AlignCenter);
  button_layout->addWidget(finish_button_, 0, Qt::AlignCenter);

  QGroupBox *appendix_group = new QGroupBox;
  if (scheduler_ == 1) {
    QHBoxLayout *appendix_layout = new QHBoxLayout;
    appendix_layout->addWidget(log_interval_label);
    appendix_layout->addWidget(log_interval_spin_box_);
    appendix_group->setTitle(tr("Appendix"));
    appendix_group->setLayout(appendix_layout);
    appendix_group->setToolTip(tr(
        "Specify the duration of the event you want to append to your diary"));
  }

  QVBoxLayout *act_layout = new QVBoxLayout;
  act_layout->addWidget(act_form_);
  QGroupBox *act_group = new QGroupBox;
  act_group->setTitle(tr("Activity"));
  act_group->setLayout(act_layout);
  act_group->setToolTip(
      tr("Describe how you were mainly working since the last diary entry"));
  act_group->setFixedHeight(act_layout->contentsMargins().top() +
                            act_layout->contentsMargins().bottom() +
                            act_layout->spacing() +
                            4 * fontMetrics().lineSpacing());

  QVBoxLayout *perf_detailed_layout = new QVBoxLayout;
  perf_detailed_layout->addWidget(perf_label);
  perf_detailed_layout->addLayout(perf_layout);
  perf_detailed_layout->addWidget(threads_label);
  perf_detailed_layout->addLayout(threads_layout);
  perf_detailed_layout->addWidget(arc_form_);
  perf_detailed_layout->addWidget(compiler_form_);
  perf_detailed_layout->addWidget(model_form_);
  perf_detailed_layout->addWidget(data_form_);
  perf_detailed_layout->setContentsMargins(0, 0, 0, 0);

  perf_widget_ = new QWidget;
  perf_widget_->setLayout(perf_detailed_layout);

  perf_group_layout_ = new QVBoxLayout;
  perf_group_layout_->addWidget(perf_section_label_);
  perf_group_layout_->addLayout(perf_buttons_);
  perf_group_layout_->addWidget(perf_widget_);
  perf_group_box_ = new QGroupBox;
  perf_group_box_->setTitle(tr("Performance"));
  perf_group_box_->setLayout(perf_group_layout_);
  perf_group_box_->setToolTip(tr("Leave at default if you did not measure the "
                                 "performance of your application."));
  perf_group_box_->setFixedHeight(
      perf_section_label_->height() + perf_checkbox1_->height() +
      perf_group_layout_->contentsMargins().top() +
      perf_group_layout_->contentsMargins().bottom() +
      perf_group_layout_->spacing() + 30 * fontMetrics().lineSpacing());

  QVBoxLayout *comment_layout = new QVBoxLayout;
  comment_layout->addWidget(comment_label);
  comment_layout->addWidget(comment_box_);
  QGroupBox *comment_group = new QGroupBox;
  comment_group->setTitle(tr("Comment"));
  comment_group->setLayout(comment_layout);
  comment_group->setToolTip(
      tr("Provide additional information of your last development activity"));
  comment_group->setFixedHeight(
      comment_label->height() + comment_box_->height() +
      comment_layout->contentsMargins().top() +
      comment_layout->contentsMargins().bottom() + comment_layout->spacing() +
      3 * fontMetrics().lineSpacing());

  ms_layout_ = new QVBoxLayout;
  ms_layout_->addWidget(ms_label_);
  ms_layout_->addLayout(ms_buttons_);
  ms_layout_->addWidget(ms_form_);
  ms_layout_->addWidget(ms_comment_label_);
  ms_layout_->addWidget(ms_comment_);
  ms_group_box_ = new QGroupBox;
  ms_group_box_->setTitle(tr("Milestone"));
  ms_group_box_->setLayout(ms_layout_);
  ms_group_box_->setToolTip(
      tr("Select yes if you reached a milestone during the "
         "last logging interval.<br>"
         "Additional questions will then appear."));
  ms_group_box_->setFixedHeight(ms_label_->height() + ms_checkbox1_->height() +
                                ms_layout_->contentsMargins().top() +
                                ms_layout_->contentsMargins().bottom() +
                                ms_layout_->spacing() +
                                4 * fontMetrics().lineSpacing());

  // Main layout and sizing
  scroll_layout_ = new QVBoxLayout;
  if (scheduler_ == 1)
    scroll_layout_->addWidget(appendix_group);
  scroll_layout_->addWidget(act_group);
  scroll_layout_->addWidget(perf_group_box_);
  comment_group->setFixedHeight(comment_group->contentsMargins().top() +
                                comment_group->contentsMargins().bottom() +
                                6 * fontMetrics().lineSpacing());
  scroll_layout_->addWidget(comment_group);
  scroll_layout_->addWidget(ms_group_box_);
  scroll_widget_ = new QWidget;
  scroll_widget_->setLayout(scroll_layout_);
  scroll_area_ = new QScrollArea;
  scroll_area_->setWidget(scroll_widget_);

  main_layout_ = new QVBoxLayout;
  main_layout_->addWidget(scroll_area_);
  main_layout_->addLayout(button_layout);

  setLayout(main_layout_);

  if (scroll_area_->verticalScrollBar()->isVisible())
    scroll_widget_->setMinimumSize(
        this->width() - scroll_area_->verticalScrollBar()->width() -
            main_layout_->contentsMargins().left() -
            main_layout_->contentsMargins().right() - 2,
        0);
  else
    scroll_widget_->setMinimumSize(
        this->width() - main_layout_->contentsMargins().left() -
            main_layout_->contentsMargins().right() - 2,
        0);
  scroll_widget_->adjustSize();

  QString title_string;
  if (scheduler_ == 1) {
    title_string = QString("Appendix to the last diary entry on %1")
                       .arg(settings.value("lastLogTime")
                                .toDateTime()
                                .toString("hh:mm ap"));
  } else {
    title_string = QString("Logging event #%1:   %2 - now")
                       .arg(no_logged_activities_ + 1)
                       .arg(settings.value("lastLogTime")
                                .toDateTime()
                                .toString("hh:mm ap"));
  }
  this->setWindowTitle(title_string);
  setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint | Qt::WindowTitleHint |
                 Qt::WindowCloseButtonHint);
  return;
}

void QuestionnaireDialog::CreateConnections() {
  QTimer *countdown_timer = new QTimer();
  connect(countdown_timer, SIGNAL(timeout()), this, SLOT(UpdateUI()));
  countdown_timer->start(60000);
  if (scheduler_ == 1)
    connect(log_interval_spin_box_, SIGNAL(valueChanged(int)), this,
            SLOT(UpdateUI()));

  connect(perf_box_, SIGNAL(activated(int)), this, SLOT(PerfInputChanged(int)));
  connect(threads_box_, SIGNAL(activated(int)), this,
          SLOT(ThreadsInputChanged(int)));
  connect(perf_group_, SIGNAL(buttonClicked(int)), this,
          SLOT(ActiveSectionsChanged()));
  connect(ms_group_, SIGNAL(buttonClicked(int)), this,
          SLOT(ActiveSectionsChanged()));

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
  connect(ms_form_->line_edit_, SIGNAL(textChanged(QString)), this,
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
  if (scheduler_ == 1) {
    int log_time = log_interval_spin_box_->value();
    if (log_time == 1) {
      info_string_ = QString("<font color='red'>*</font> 1. What were you "
                             "working on the last minute?");
    } else {
      info_string_ = QString("<font color='red'>*</font> 1. What were you "
                             "working on the last %1 minutes?")
                         .arg(log_time);
    }
  } else {
    if (interval == 1) {
      info_string_ = QString("<font color='red'>*</font> 1. What were you "
                             "working on the last minute?");
    } else {
      info_string_ = QString("<font color='red'>*</font> 1. What were you "
                             "working on the last %1 minutes?")
                         .arg(interval);
    }
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
  if (scheduler_ == 1) {
    activity_->SetIntervalTime(log_interval_spin_box_->value());
  } else {
    activity_->SetIntervalTime(
        qCeil(cur_time.secsTo(activity_->GetLastTime()) / -60.0));
  }
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

  activity_->SetComment(comment_box_->toPlainText());

  if (ms_group_->checkedId() == 1) {
    activity_->SetMsTitle(ms_form_->GetText());
    activity_->SetMsComment(ms_comment_->toPlainText());
    if (scheduler_ != 1)
      activity_->SetMsId(project_->GetNoMilestones());
  } else {
    activity_->SetMsId(-1);
  }
  switch (scheduler_) {
  case 0:
    activity_->SetScheduler(0);
    break;
  case 1:
    activity_->SetScheduler(1);
    break;
  case 2:
    activity_->SetScheduler(2);
    break;
  case 3:
    activity_->SetScheduler(3);
    break;
  default:
    activity_->SetScheduler(-1);
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
  if (scheduler_ == 1) {
    main_window_->AddAppendix(*activity_);
  } else {
    activity_->SetId(project_->GetNoActivities());
    project_->AddActivity(*activity_);
    project_->StoreLog(settings.value("conf/logFile").toString());
    main_window_->SetupAnimation();
  }
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

void QuestionnaireDialog::ActiveSectionsChanged() {
  bool perf_act = false;
  if (perf_group_->checkedId() == 1)
    perf_act = true;
  bool ms_act = false;
  if (ms_group_->checkedId() == 1)
    ms_act = true;

  if (perf_act) {
    perf_widget_->show();
    perf_group_layout_->update();
    perf_group_box_->setFixedHeight(
        perf_section_label_->height() + perf_checkbox1_->height() +
        perf_group_layout_->contentsMargins().top() +
        perf_group_layout_->contentsMargins().bottom() +
        perf_group_layout_->spacing() + 30 * fontMetrics().lineSpacing());
  } else {
    perf_widget_->hide();
    perf_group_layout_->update();
    perf_group_box_->setFixedHeight(
        perf_section_label_->height() + perf_checkbox1_->height() +
        perf_group_layout_->contentsMargins().top() +
        perf_group_layout_->contentsMargins().bottom() +
        perf_group_layout_->spacing() + 4 * fontMetrics().lineSpacing());
  }

  if (ms_act) {
    ms_form_->show();
    ms_comment_label_->show();
    ms_comment_->show();
    ms_form_->setFocus();
    ms_group_box_->setFixedHeight(
        ms_label_->height() + ms_checkbox1_->height() +
        ms_layout_->contentsMargins().top() +
        ms_layout_->contentsMargins().bottom() + ms_layout_->spacing() +
        10 * fontMetrics().lineSpacing());
  } else {
    ms_form_->hide();
    ms_comment_label_->hide();
    ms_comment_->hide();
    ms_group_box_->setFixedHeight(
        ms_label_->height() + ms_checkbox1_->height() +
        ms_layout_->contentsMargins().top() +
        ms_layout_->contentsMargins().bottom() + ms_layout_->spacing() +
        4 * fontMetrics().lineSpacing());
  }
  scroll_widget_->adjustSize();
  main_layout_->update();
  this->repaint();
  QApplication::processEvents();
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
    if (ms_form_->GetText().isEmpty()) {
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

void QuestionnaireDialog::resizeEvent(QResizeEvent *event) {
  QDialog::resizeEvent(event);
  if (scroll_area_->verticalScrollBar()->isVisible())
    scroll_widget_->setMinimumSize(
        this->width() - scroll_area_->verticalScrollBar()->width() -
            main_layout_->contentsMargins().left() -
            main_layout_->contentsMargins().right() - 2,
        0);
  else
    scroll_widget_->setMinimumSize(
        this->width() - main_layout_->contentsMargins().left() -
            main_layout_->contentsMargins().right() - 2,
        0);
  scroll_widget_->adjustSize();
  return;
}
