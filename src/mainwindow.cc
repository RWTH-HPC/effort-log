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

#include "mainwindow.h"
#include "definitions.h"
#include "questionnairedialog.h"
#ifdef __APPLE__
#include "appnap.h"
#endif
#include "logview.h"

#include <QDebug>
#include <QFileDialog>

MainWindow::MainWindow() : QMainWindow() {
  log_running_ = false;
  qst_running_ = false;
  appendices_ = new QList<Activity>;
  Setup();
  CreateActions();
  CreateConnections();
  CreateMenus();
#ifdef __APPLE__
  disableAppNap();
#endif
}

#ifdef CRYPT
MainWindow::MainWindow(Crypt *crypt) : QMainWindow() {
  crypt_ = crypt;
  log_running_ = false;
  qst_running_ = false;
  appendices_ = new QList<Activity>;
  Setup();
  CreateActions();
  CreateConnections();
  CreateMenus();
#ifdef __APPLE__
  disableAppNap();
#endif
}
#endif

void MainWindow::closeEvent(QCloseEvent *event) {
  settings_.setValue("geometry", saveGeometry());
  settings_.setValue("windowState", saveState());
  if (settings_.value("conf/confAccepted") == true) {
    QMessageBox::StandardButton button;
    button = QMessageBox::question(this, APP_NAME,
                                   tr("Are you sure you want "
                                      "to stop logging your effort?\n"),
                                   QMessageBox::Cancel | QMessageBox::Yes,
                                   QMessageBox::Yes);
    if (button != QMessageBox::Yes) {
      event->ignore();
    } else {
      if (settings_.value("noLoggedActivities").toInt() > 0) {
        int cur_interval =
            QTime::currentTime().msecsSinceStartOfDay() -
            settings_.value("lastLogTime").toTime().msecsSinceStartOfDay();
        if (VERBOSE) {
          qDebug() << cur_interval;
          qDebug() << (settings_.value("conf/logInterval").toInt() * 60000 *
                       LOG_ON_EXIT_THRESHOLD);
        }
        if (cur_interval > 1 &&
            cur_interval > (settings_.value("conf/logInterval").toInt() *
                            60000 * LOG_ON_EXIT_THRESHOLD)) {
          countdown_timer_->stop();
          qst_running_ = true;
          QuestionnaireDialog dialog(this, 3);
          dialog.exec();
          dialog.show();
          dialog.raise();
          dialog.setWindowModality(Qt::WindowModal);
        }
        project_->StoreLog(settings_.value("conf/logFile").toString());
      }
      tray_icon_->hide();
      event->accept();
    }
  } else {
    tray_icon_->hide();
    event->accept();
  }
}

void MainWindow::Setup() {
  tray_icon_ = new QSystemTrayIcon();
  tray_icon_->setIcon(QIcon(":/images/hourglass.png"));
  tray_icon_->show();
  QString tooltip;
  tooltip.append(tr("%1 %2").arg(APP_NAME, APP_VERSION));
  tray_icon_->setToolTip(tooltip);

  // Status bar
  status_bar_ = this->statusBar();
  status_label_running_ = new QLabel();
  status_label_time_ = new QLabel();
  status_progress_bar_ = new QProgressBar();
  statusBar()->addWidget(status_label_running_);

  tool_bar_ = new QToolBar(tr("MainToolBar"), this);
  tool_bar_->setObjectName("MainToolBar");
  project_label_ = new QLabel;

  status_progress_bar_->setValue(100);
  this->addToolBar(tool_bar_);
  tool_bar_->setMovable(false);
  restoreGeometry(settings_.value("geometry").toByteArray());
  restoreState(settings_.value("windowState").toByteArray());
}

void MainWindow::CreateActions() {
  log_action_ =
      new QAction(QIcon(":/images/log.png"), tr("&Read current log"), this);
  log_action_->setShortcut(QKeySequence::Open);
  log_action_->setStatusTip(tr("Open the current log file"));
  new_action_ =
      new QAction(QIcon(":/images/new.png"), tr("&Log current effort"), this);
  new_action_->setShortcut(QKeySequence::New);
  new_action_->setStatusTip(tr("Create a new logging event"));
  help_action_ = new QAction(tr("&Help"), this);
  help_action_->setShortcut(QKeySequence::HelpContents);
  help_action_->setStatusTip(tr("Open the help file"));
  about_action_ = new QAction(tr("&About"), this);
  about_action_->setStatusTip(tr("About this program"));
}

void MainWindow::CreateConnections() {
  countdown_timer_ = new QTimer();  // Countdown until the next event
  connect(countdown_timer_, SIGNAL(timeout()), this,
          SLOT(NextAnimationFrame()));
  connect(log_action_, SIGNAL(triggered()), this, SLOT(LogViewer()));
  connect(new_action_, SIGNAL(triggered()), this,
          SLOT(ExecQuestionnaireDialog()));
  connect(help_action_, SIGNAL(triggered()), this, SLOT(Help()));
  connect(about_action_, SIGNAL(triggered()), this, SLOT(About()));
}

void MainWindow::UpdateUI() {
  if (log_running_ == 1) {
    status_label_running_->setText(tr("Running"));
  } else {
    status_label_running_->setText(tr("Not running"));
  }
  countdown_time_.setHMS(0, settings_.value("conf/logInterval").toInt(), 0);
  setWindowTitle(tr("Effort log"));
  project_label_->setText(
      "Active project:    " + settings_.value("conf/projectTitle").toString() +
      "<br>Project directory: " +
      settings_.value("conf/projectDir").toString());
}

void MainWindow::SetupAnimation() {
  double tmp = settings_.value("conf/logInterval").toDouble() * 60.0;
  status_progress_bar_->setMaximum(static_cast<int>(tmp));
  status_progress_bar_->setValue(static_cast<int>(tmp));
  status_progress_bar_->resize(10, 500);
  statusBar()->addWidget(status_progress_bar_, 1);
  statusBar()->addWidget(status_label_time_);
  QTime time = QTime::currentTime();
  time = time.addSecs(settings_.value("conf/logInterval").toInt() * 60);
  QString s = tr("Next:   ");
  s += time.toString("hh:mm ap");
  SetTimeLabel(s.left(16));
  countdown_timer_->start(1000);
}

void MainWindow::QuitOnSignal() {
  settings_.setValue("geometry", saveGeometry());
  settings_.setValue("windowState", saveState());
  if (settings_.value("noLoggedActivities").toInt() > 0) {
    int cur_interval =
        QTime::currentTime().msecsSinceStartOfDay() -
        settings_.value("lastLogTime").toTime().msecsSinceStartOfDay();
    if (VERBOSE) {
      qDebug() << cur_interval;
      qDebug() << (settings_.value("conf/logInterval").toInt() * 60000 *
                   LOG_ON_EXIT_THRESHOLD);
    }
    if (cur_interval > 1 &&
        cur_interval > (settings_.value("conf/logInterval").toInt() * 60000 *
                        LOG_ON_EXIT_THRESHOLD)) {
      countdown_timer_->stop();
      qst_running_ = true;
      QuestionnaireDialog dialog(this, 3);
      dialog.exec();
      dialog.show();
      dialog.setWindowModality(Qt::WindowModal);
    }
    project_->StoreLog(settings_.value("conf/logFile").toString());
  }
}

void MainWindow::NextAnimationFrame() {
  if ((countdown_time_.hour() == 0) && (countdown_time_.minute() == 0) &&
      (countdown_time_.second() == 0)) {
    if (qst_running_ == false) {
      qst_running_ = true;
      QString msg = tr("Your specified interval of ");
      msg += settings_.value("conf/logInterval").toString();
      if (settings_.value("conf/logInterval").toInt() == 1)
        msg += " minute";
      else
        msg += " minutes";
      msg +=
          " has passed. Please switch to the EffortLog app and answer the "
          "questionnaire.";
      ShowMessage(msg);
      QApplication::processEvents();
      QApplication::sendPostedEvents(tray_icon_);
      QuestionnaireDialog dialog(this, 0);
      dialog.exec();
      dialog.show();
      dialog.setWindowModality(Qt::WindowModal);
      countdown_time_.setHMS(0, settings_.value("conf/logInterval").toInt(), 0);
    }
  } else {
    countdown_time_ = countdown_time_.addSecs(-1);
  }
  if (log_running_ == true) {
    if (status_progress_bar_->value() != 0) {
      status_progress_bar_->setValue(status_progress_bar_->value() - 1);
    }
  }
}

void MainWindow::CreateMenus() {
  QMenu *help_menu = new QMenu(tr("&Help"), this);
  help_menu->addAction(help_action_);
  help_menu->addAction(about_action_);
  QMenu *file_menu = new QMenu(tr("&File"), this);
  file_menu->addAction(new_action_);
#ifndef CRYPT
  file_menu->addAction(log_action_);
  menuBar()->addMenu(file_menu);
#endif
  menuBar()->addMenu(help_menu);

  tray_icon_->setContextMenu(file_menu);

  QWidget *spacer = new QWidget();
  spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  tool_bar_->addAction(log_action_);
  tool_bar_->addAction(new_action_);
  tool_bar_->addWidget(spacer);
  tool_bar_->addWidget(project_label_);
}

void MainWindow::SetLogRunning(bool r) { log_running_ = r; }

void MainWindow::SetQstRunning(bool r) { qst_running_ = r; }

void MainWindow::SetTimeLabel(QString s) { status_label_time_->setText(s); }

void MainWindow::LogViewer() {
  LogView view(project_);
  view.resize(450, 300);
  view.exec();
}

void MainWindow::About() {
  QString msg;
  msg.append(tr("<h1>%1 %2</h1>").arg(APP_NAME, APP_VERSION));
  msg.append("<br/>Copyright © 2015-2021 by IT Center");
  msg.append("<br/>Group: High Performance Computing");
  msg.append("<br/>Division: Computational Science and Engineering");
  msg.append("<br/>RWTH Aachen University");
  msg.append("<br/>Seffenter Weg 23");
  msg.append("<br/>52074 Aachen, Germany");
  msg.append(
      "<br/><a "
      "href='www.hpc.rwth-aachen.de/research/tco/"
      "'>www.hpc.rwth-aachen.de/research/tco</a>");

  QMessageBox about_box(this);
  about_box.setText(msg);
  about_box.setTextFormat(Qt::RichText);
  about_box.setTextInteractionFlags(Qt::TextBrowserInteraction);
  QFont font = about_box.font();
  font.setPointSize(11);
  font.setBold(0);
  about_box.setFont(font);
  about_box.setStandardButtons(QMessageBox::Close);
  about_box.setDefaultButton(QMessageBox::Close);
  about_box.setWindowTitle("About Effort Log");
  about_box.exec();
}

void MainWindow::Help() {
  QTextBrowser *browser = new QTextBrowser();
  browser->setSource(QUrl("qrc:/doc.html"));
  browser->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
  browser->setWindowTitle(tr("%1 help").arg(APP_NAME));
  browser->resize(600, 800);
  browser->show();
}

void MainWindow::ReadLog() {
  project_->ReadLog(
      QDir::toNativeSeparators(settings_.value("conf/logFile").toString()));
}

void MainWindow::SetProject(Project *p) { project_ = p; }

void MainWindow::ExecQuestionnaireDialog() {
  qst_running_ = true;
  QuestionnaireDialog dialog(this, 2);
  dialog.exec();
  dialog.show();
  dialog.setWindowModality(Qt::WindowModal);
}

void MainWindow::ExecScheduledQuestionnaireDialog() {
  qst_running_ = true;
  QuestionnaireDialog dialog(this, 0);
  dialog.exec();
  dialog.show();
  dialog.setWindowModality(Qt::WindowModal);
}

void MainWindow::AddAppendix(Activity act) { appendices_->append(act); }

void MainWindow::AddAppendices() {
  while (appendices_->length() > 0) {
    appendices_->first().SetId(project_->GetNoActivities());
    if (appendices_->first().GetMsTitle() != "")
      appendices_->first().SetMsId(project_->GetNoMilestones());
    project_->AddActivity(appendices_->first());
    appendices_->removeFirst();
  }
  project_->StoreLog(settings_.value("conf/logFile").toString());
}

void MainWindow::ShowMessage(const QString &msg) {
  QString app = tr(APP_NAME) + " " + tr(APP_VERSION);
  tray_icon_->showMessage(app, msg);
}
