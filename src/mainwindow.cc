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

#include "definitions.h"
#include "questionnairedialog.h"
#include "mainwindow.h"
#ifdef __APPLE__
#include "appnap.h"
#endif

#include <QDebug>
#include <QFileDialog>


MainWindow::MainWindow() : QMainWindow() {
  log_running_ = false;
  Setup();
  CreateActions();
  CreateConnections();
  CreateMenus();
#ifdef __APPLE__
  disableAppNap();
#endif
}

MainWindow::MainWindow(Crypt *crypt) : QMainWindow() {
  crypt_ = crypt;
  log_running_ = false;
  Setup();
  CreateActions();
  CreateConnections();
  CreateMenus();
#ifdef __APPLE__
  disableAppNap();
#endif
}

void MainWindow::closeEvent (QCloseEvent *event) {
  settings_.setValue("geometry", saveGeometry());
  settings_.setValue("windowState", saveState());
  if (settings_.value("conf/confAccepted") == true) {
    QMessageBox::StandardButton button;
    button = QMessageBox::question(this, APP_NAME, tr("Are you sure you want "
                                   "to stop logging your effort?\n"),
                                   QMessageBox::Cancel | QMessageBox::Yes,
                                   QMessageBox::Yes);
    if (button != QMessageBox::Yes) {
      event->ignore();
    } else {
      if (settings_.value("noLoggedActivities").toInt() > 0) {
        int cur_interval = QTime::currentTime().msecsSinceStartOfDay() -
            settings_.value("lastLogTime").toTime().msecsSinceStartOfDay();
        if (VERBOSE) {
          qDebug() << cur_interval;
          qDebug() << (settings_.value("conf/logInterval").toInt() * 60000
                       * LOG_ON_EXIT_THRESHOLD);
        }
        if (cur_interval > 1
            && cur_interval > (settings_.value("conf/logInterval").toInt()
                               * 60000 * LOG_ON_EXIT_THRESHOLD)) {
          countdown_timer_->stop();
          QuestionnaireDialog dialog(this);
          dialog.exec();
          dialog.show();
          dialog.setWindowModality(Qt::WindowModal);
        }
        project_->StoreLog(settings_.value("conf/logFile").toString());
      }
      event->accept();
    }
  } else {
    event->accept();
  }
}

void MainWindow::Setup() {
  //central_widget_ = new QWidget();

  // Status bar
  status_bar_ = this->statusBar();
  status_label_running_ = new QLabel();
  status_label_time_ = new QLabel();
  status_progress_bar_ = new QProgressBar();
  statusBar()->addWidget(status_label_running_);

  tool_bar_ = new QToolBar(this);
  project_label_ = new QLabel;

  status_progress_bar_->setValue(100);
  this->addToolBar(tool_bar_);
  tool_bar_->setMovable(false);
  restoreGeometry(settings_.value("geometry").toByteArray());
  restoreState(settings_.value("windowState").toByteArray());
}

void MainWindow::CreateActions() {
  log_action_ = new QAction(QIcon(":/images/log.png"), tr("&Read current log"),
                            this);
  log_action_->setShortcut(QKeySequence::Open);
  log_action_->setStatusTip(tr("Open the current log file"));
  help_action_ = new QAction(QIcon(":/images/help_browser.png"), tr("&Help"),
                             this);
  help_action_->setShortcut(QKeySequence::HelpContents);
  help_action_->setStatusTip(tr("Open the help file"));
  about_action_ = new QAction(QIcon(":/images/about.png"), tr("&About"), this);
  about_action_->setStatusTip(tr("About this program"));
}


void MainWindow::CreateConnections() {
  countdown_timer_ = new QTimer();  // Countdown until the next event
  connect(countdown_timer_, SIGNAL(timeout()), this,
          SLOT(NextAnimationFrame()));
  connect(log_action_, SIGNAL(triggered()), this, SLOT(LogView()));
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
  project_label_->setText("Active project: "
                          + settings_.value("conf/projectTitle").toString()
                          + "     ");
  //central_widget_->update();
}

void MainWindow::SetupAnimation() {
  double tmp = settings_.value("conf/logInterval").toDouble() * 60.0;
  status_progress_bar_->setMaximum(tmp);
  status_progress_bar_->setValue(tmp);
  status_progress_bar_->resize(10,500);
  statusBar()->addWidget(status_progress_bar_,1);
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
    int cur_interval = QTime::currentTime().msecsSinceStartOfDay() -
        settings_.value("lastLogTime").toTime().msecsSinceStartOfDay();
    if (VERBOSE) {
      qDebug() << cur_interval;
      qDebug() << (settings_.value("conf/logInterval").toInt() * 60000
                   * LOG_ON_EXIT_THRESHOLD);
    }
    if (cur_interval > 1
        && cur_interval > (settings_.value("conf/logInterval").toInt()
                           * 60000 * LOG_ON_EXIT_THRESHOLD)) {
      countdown_timer_->stop();
      QuestionnaireDialog dialog(this);
      dialog.exec();
      dialog.show();
      dialog.setWindowModality(Qt::WindowModal);
    }
    project_->StoreLog(settings_.value("conf/logFile").toString());
  }
}

void MainWindow::NextAnimationFrame() {
  if ((countdown_time_.hour() == 0) && (countdown_time_.minute() == 0)
       && (countdown_time_.second() == 0)) {
    QuestionnaireDialog dialog(this);
    dialog.exec();
    dialog.show();
    dialog.setWindowModality(Qt::WindowModal);
    countdown_time_.setHMS(0, settings_.value("conf/logInterval").toInt(), 0);
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
#ifndef CRYPT
  // TODO: Display encrypted log files
  QMenu *file_menu = new QMenu(tr("&File"), this);
  file_menu->addAction(log_action_);
  menuBar()->addMenu(file_menu);
#endif
  menuBar()->addMenu(help_menu);

  QWidget* spacer = new QWidget();
  spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
#ifndef CRYPT
  tool_bar_->addAction(log_action_);
#endif
  tool_bar_->addAction(help_action_);
  tool_bar_->addAction(about_action_);
  tool_bar_->addWidget(spacer);
  tool_bar_->addWidget(project_label_);
}

void MainWindow::SetLogRunning(bool r) {
  log_running_ = r;
}

void MainWindow::SetTimeLabel(QString s) {
  status_label_time_->setText(s);
}

void MainWindow::LogView() {
  QFile file(settings_.value("conf/logFile").toString());
  file.open(QFile::ReadOnly | QFile::Text);
  QTextStream ReadFile(&file);
  QTextEdit * textEdit = new QTextEdit;
  textEdit->setText(ReadFile.readAll());
  textEdit->resize(400, 500);
  textEdit->setWindowTitle(settings_.value("conf/logFile").toString());
  textEdit->show();
}

void MainWindow::About() {
  QMessageBox about_box;
  QSpacerItem* horizontalSpacer = new QSpacerItem(300, 0, QSizePolicy::Minimum,
                                                  QSizePolicy::Expanding);
  about_box.setText(tr("<center>%1 %2</center>").arg(APP_NAME)
                       .arg(APP_VERSION));
  about_box.setInformativeText("Copyright © 2016 by IT Center\n"
                               "Group: High Performance Computing\n"
                               "Division: Computational Science and Engineering"
                               "\nRWTH Aachen University\n"
                               "Seffenter Weg 23\n"
                               "52074 Aachen, Germany");
  about_box.setStandardButtons(QMessageBox::Close);
  about_box.setDefaultButton(QMessageBox::Close);
  about_box.setWindowTitle("About Effort Log");
  // Add space to increae the box size
  QGridLayout* layout = (QGridLayout*)about_box.layout();
  layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1,
                  layout->columnCount());
  about_box.exec();
}

void MainWindow::Help() {
  QTextBrowser *browser = new QTextBrowser();
  browser->setSource(QUrl("qrc:///index.html"));
  browser->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
  browser->setWindowTitle(tr("%1 help").arg(APP_NAME));
  browser->resize(400, 500);
  browser->show();
}

void MainWindow::ReadLog() {
  project_->ReadLog(settings_.value("conf/logFile").toString());
}

void MainWindow::SetProject(Project *p) {
  project_ = p;
}
