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

#include "definitions.h"
#ifdef CRYPT
#include "passworddialog.h"
#endif
#include "proinitdialog.h"
#include "setupdialog.h"

#include <QCompleter>
#include <QDebug>
#include <QFileInfo>
#include <QStringListModel>

SetupDialog::SetupDialog(MainWindow *window) : QDialog() {
  main_window_ = window;
  project_ = new Project();
  new_project_flag_ = true;
  settings_.setValue("conf/append", false);
  Setup();
  CreateConnections();
  LoadSettings();
}

#ifdef CRYPT
SetupDialog::SetupDialog(MainWindow *window, Crypt *crypt) : QDialog() {
  main_window_ = window;
  crypt_ = crypt;
  project_ = new Project(crypt);
  new_project_flag_ = true;
  settings_.setValue("conf/append", false);
  Setup();
  CreateConnections();
  LoadSettings();
}
#endif

void SetupDialog::Setup() {
  layout_ = new QGridLayout;
  QGridLayout *project_layout = new QGridLayout;
  QGridLayout *log_layout = new QGridLayout;
  QVBoxLayout *app_layout = new QVBoxLayout;

  // Specify project title
  QLabel *project_title_label = new QLabel(tr("Project title:"));
  project_title_line_edit_ = new QLineEdit();
  project_title_line_edit_->setToolTip(tr("Specify the project title"));
  completer_ = new QCompleter(dir_list_);
  project_title_line_edit_->setCompleter(completer_);

  QLabel *project_dir_label = new QLabel(tr("Project directory:"));
  project_dir_line_edit_ = new QLineEdit();
  project_dir_line_edit_->setToolTip(tr("Specify the working directory of the "
                                        "project"));
  project_dir_line_edit_->setText(
      QDir::toNativeSeparators(QDir::currentPath()));

  browse_pro_button_ = new QPushButton(tr("Browse..."));
  browse_pro_button_->setToolTip(tr("Browse an output directory"));

  // Specify user name
  QLabel *user_name_label = new QLabel(tr("User name:"));
  user_name_line_edit_ = new QLineEdit();
  user_name_line_edit_->setToolTip(tr("Specify the user name"));
  user_name_line_edit_->setText(QDir::home().dirName());
  user_name_line_edit_->setPlaceholderText(QDir::home().dirName());

  // Specify logging interval
  QLabel *log_interval_label = new QLabel(tr("Logging interval:"));
  log_interval_spin_box_ = new QSpinBox;
  log_interval_spin_box_->setRange(MIN_LOG_INTERVAL, MAX_LOG_INTERVAL);
  log_interval_spin_box_->setSingleStep(1);
  log_interval_spin_box_->setValue(15);
  log_interval_spin_box_->setToolTip(tr("Set the logging interval"
                                        "between %1 and %2 minutes")
                                         .arg(MIN_LOG_INTERVAL)
                                         .arg(MAX_LOG_INTERVAL));
  log_interval_spin_box_->setSuffix(" min");

  // Specify the log file's output directory
  QLabel *log_file_dir_label = new QLabel(tr("Output directory:"));
  log_file_dir_line_edit_ = new QLineEdit;
  log_file_dir_line_edit_->setText(
      QDir::toNativeSeparators(QDir::currentPath()));
  log_file_dir_line_edit_->setToolTip(tr("Specify the output directory"));
  log_file_dir_label->setBuddy(log_file_dir_line_edit_);

  // Specify the log file's name
  QLabel *log_file_name_label = new QLabel(tr("Log file name:"));
  log_file_name_line_edit_ = new QLineEdit;
  log_file_name_label->setBuddy(log_file_name_line_edit_);
  log_file_name_line_edit_->setToolTip(tr("Specify the name of the log file"));
  browse_log_button_ = new QPushButton(tr("Browse..."));
  browse_log_button_->setToolTip(tr("Browse an output directory"));

  buttons_ = new QDialogButtonBox(
      QDialogButtonBox::Ok | QDialogButtonBox::Close, Qt::Horizontal);
  buttons_->button(QDialogButtonBox::Ok)->setEnabled(false);

  // Appendix
  QLabel *app_label = new QLabel(tr("Do you want to append the work you have "
                                    "done before executing this program to the "
                                    "above specified project?"));
  app_label->setWordWrap(true);
  app_button_ = new QPushButton(tr("Append..."));
  app_button_->setEnabled(false);
  app_button_->setToolTip(tr("Append current work"));

  // Layout
  project_layout->addWidget(project_title_label, 0, 0);
  project_layout->addWidget(project_title_line_edit_, 0, 1);
  project_layout->addWidget(project_dir_label, 1, 0);
  project_layout->addWidget(project_dir_line_edit_, 1, 1);
  project_layout->addWidget(browse_pro_button_, 1, 2);
  project_layout->addWidget(user_name_label, 2, 0);
  project_layout->addWidget(user_name_line_edit_, 2, 1);

  log_layout->addWidget(log_file_dir_label, 0, 0);
  log_layout->addWidget(log_file_dir_line_edit_, 0, 1);
  log_layout->addWidget(browse_log_button_, 0, 2);
  log_layout->addWidget(log_file_name_label, 1, 0);
  log_layout->addWidget(log_file_name_line_edit_, 1, 1);
  log_layout->addWidget(log_interval_label, 2, 0);
  log_layout->addWidget(log_interval_spin_box_, 2, 1);

  app_layout->addWidget(app_label);
  app_layout->addWidget(app_button_);

  QGroupBox *project_box = new QGroupBox(tr("Project Setup"));
  QGroupBox *log_box = new QGroupBox(tr("Logging Setup"));
  QGroupBox *app_box = new QGroupBox(tr("Did you work before executing this "
                                        "program and want to log it?"));
  project_box->setLayout(project_layout);
  log_box->setLayout(log_layout);
  app_box->setLayout(app_layout);
  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget(project_box);
  layout->addWidget(log_box);
  layout->addWidget(app_box);
  layout->addWidget(buttons_);

  this->setLayout(layout);
  this->setWindowFlags(Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
  this->setWindowTitle(tr("Configuration"));
  return;
}

void SetupDialog::CreateConnections() {
  connect(browse_pro_button_, SIGNAL(clicked()), this, SLOT(BrowsePro()));
  connect(browse_log_button_, SIGNAL(clicked()), this, SLOT(BrowseDir()));
  connect(app_button_, SIGNAL(clicked()), this, SLOT(Append()));

  connect(project_title_line_edit_, SIGNAL(textChanged(QString)), this,
          SLOT(SetLogFileName()));
  connect(project_dir_line_edit_, SIGNAL(textChanged(QString)), this,
          SLOT(AutoComplete()));

  connect(user_name_line_edit_, SIGNAL(textChanged(QString)), this,
          SLOT(CheckInput()));
  connect(project_title_line_edit_, SIGNAL(textChanged(QString)), this,
          SLOT(CheckInput()));
  connect(project_dir_line_edit_, SIGNAL(textChanged(QString)), this,
          SLOT(CheckInput()));
  connect(log_file_dir_line_edit_, SIGNAL(textChanged(QString)), this,
          SLOT(CheckInput()));
  connect(log_file_name_line_edit_, SIGNAL(textChanged(QString)), this,
          SLOT(CheckInput()));

  connect(buttons_, SIGNAL(accepted()), this, SLOT(accept()));
  connect(buttons_, SIGNAL(rejected()), this, SLOT(reject()));
}

void SetupDialog::accept() {
  QDir dir(project_dir_line_edit_->text());
  if (!dir.exists()) {
    const QString text = "The working directory '" +
                         project_dir_line_edit_->text() +
                         "' does not exist. Please specify "
                         "another one.";
    QMessageBox::warning(this, "Directory does not exist", text,
                         QMessageBox::Ok);
    return;
  } else {
    QFileInfo finfo(dir.absoluteFilePath(project_dir_line_edit_->text()));
    if (!finfo.isWritable()) {
      const QString text = "The working directory '" +
                           project_dir_line_edit_->text() +
                           "' is not writable. Please "
                           "specify another one.";
      QMessageBox::warning(this, "Directory not writable", text,
                           QMessageBox::Ok);
      return;
    }
  }
  dir = QDir(log_file_dir_line_edit_->text());
  if (!dir.exists()) {
    const QString text = "The log file directory '" +
                         log_file_dir_line_edit_->text() +
                         "' does not exist. Please specify "
                         "another one.";
    QMessageBox::warning(this, "Directory does not exist", text,
                         QMessageBox::Ok);
    return;
  } else {
    QFileInfo finfo(dir.absoluteFilePath(log_file_dir_line_edit_->text()));
    if (!finfo.isWritable()) {
      const QString text = "The log file directory '" +
                           log_file_dir_line_edit_->text() +
                           "' is not writable. Please "
                           "specify another one.";
      QMessageBox::warning(this, "Directory not writable", text,
                           QMessageBox::Ok);
      return;
    }
  }

  settings_.setValue("conf/projectTitle", project_title_line_edit_->text());
  settings_.setValue("conf/userName", user_name_line_edit_->text());
  settings_.setValue("conf/logFileName", log_file_name_line_edit_->text());
  settings_.setValue("conf/logFileDir", log_file_dir_line_edit_->text());
  QString fileName = settings_.value("conf/logFileDir").toString() + "/" +
                     settings_.value("conf/logFileName").toString();
  settings_.setValue("conf/logFile", fileName);
  settings_.setValue("conf/logInterval", log_interval_spin_box_->value());
  settings_.setValue("conf/confAccepted", true);
  settings_.setValue("lastLogTime", QDateTime::currentDateTime());

  if (VERBOSE)
    qDebug() << "LastLogTime" << settings_.value("lastLogTime");

  if (ProjectHandler() == true) {
    if (new_project_flag_ == true) {
#ifdef CRYPT
      PasswordDialog pwd(crypt_, true);
      if (pwd.exec() == QDialog::Rejected) {
        return;
      }
#endif
      ProInitDialog d(project_);
      if (d.exec() == QDialog::Rejected) {
        return;
      }
      project_->AddContributor(user_name_line_edit_->text());
      project_->SetProDir(project_dir_line_edit_->text());
      project_->SetTitle(project_title_line_edit_->text());
      project_->SetInterval(log_interval_spin_box_->value());
      project_->SetLogDir(log_file_dir_line_edit_->text());
      project_->SetLogFile(log_file_name_line_edit_->text());
    }

    if (!project_dir_.isEmpty()) {
      QStringList tmp = project_->GetContributor();
      foreach (const QString &str, tmp) {
        if (QString::compare(str, user_name_line_edit_->text()) != 0)
          project_->AddContributor(user_name_line_edit_->text());
      }
      if (QString::compare(project_->GetProDir(),
                           project_dir_line_edit_->text()) != 0)
        project_->AddContributor(project_dir_line_edit_->text());
      if (QString::compare(project_->GetTitle(),
                           project_title_line_edit_->text()) != 0)
        project_->AddContributor(project_title_line_edit_->text());
      if (project_->GetInterval() != log_interval_spin_box_->value())
        project_->SetInterval(log_interval_spin_box_->value());
      project_->Save(project_dir_);
    } else {
      qWarning("No working directory specified!");
    }
    QFileInfo checkFile(
        QDir::toNativeSeparators(settings_.value("conf/logFile").toString()));
    if (new_project_flag_ == false && checkFile.exists() &&
        checkFile.isFile()) {
      project_->ReadLog(
          QDir::toNativeSeparators(settings_.value("conf/logFile").toString()));
    }

    main_window_->SetProject(project_);
    main_window_->AddAppendices();
    main_window_->SetLogRunning(true);
    main_window_->SetupAnimation();
    main_window_->UpdateUI();
    main_window_->show();
    SaveSettings();
    QDialog::accept();
  } else {
    reject();
  }
  return;
}

void SetupDialog::reject() {
  QMessageBox::StandardButton button;
  button = QMessageBox::question(this, APP_NAME,
                                 tr("Are you sure you want to "
                                    "quit this program?\n"),
                                 QMessageBox::Cancel | QMessageBox::Yes,
                                 QMessageBox::Yes);
  if (button != QMessageBox::Cancel) {
    QDialog::reject();
    settings_.setValue("conf/confAccepted", false);
    main_window_->close();
  }
  return;
}

void SetupDialog::LoadSettings() {
  if (settings_.contains("conf/userName"))
    user_name_line_edit_->setText(settings_.value("conf/userName").toString());
  if (settings_.contains("conf/projectTitle"))
    project_title_line_edit_->setText(
        settings_.value("conf/projectTitle").toString());
  if (settings_.contains("conf/projectDir"))
    project_dir_line_edit_->setText(
        settings_.value("conf/projectDir").toString());
  if (settings_.contains("conf/logFileDir"))
    log_file_dir_line_edit_->setText(
        settings_.value("conf/logFileDir").toString());
  if (settings_.contains("conf/logFileName"))
    log_file_name_line_edit_->setText(
        settings_.value("conf/logFileName").toString());
  if (settings_.contains("conf/logInterval"))
    log_interval_spin_box_->setValue(
        settings_.value("conf/logInterval").toInt());
  return;
}

void SetupDialog::SaveSettings() {
  settings_.setValue("conf/projectTitle", project_title_line_edit_->text());
  settings_.setValue("conf/projectDir", project_dir_line_edit_->text());
  settings_.setValue("conf/userName", user_name_line_edit_->text());
  settings_.setValue("conf/logFileName", log_file_name_line_edit_->text());
  settings_.setValue("conf/logFileDir", log_file_dir_line_edit_->text());
  QString fileName = settings_.value("conf/logFileDir").toString() + "/" +
                     settings_.value("conf/logFileName").toString();
  settings_.setValue("conf/logFile", fileName);
  settings_.setValue("conf/logInterval", log_interval_spin_box_->value());
  return;
}

void SetupDialog::BrowseDir() {
  dir_string_ = QFileDialog::getExistingDirectory(
      this, tr("Save log file to..."), QDir::currentPath());
  if (!dir_string_.isEmpty())
    log_file_dir_line_edit_->setText(dir_string_);
  return;
}

void SetupDialog::BrowsePro() {
  QString dir = QFileDialog::getExistingDirectory(
      this, tr("Save project to..."), QDir::currentPath());
  if (!dir.isEmpty()) {
    project_dir_ = dir;
    project_dir_line_edit_->setText(dir);
  }
  return;
}

void SetupDialog::SetLogFileName() {
  log_file_name_line_edit_->setText(project_title_line_edit_->text() +
                                    tr(".json"));
  return;
}

void SetupDialog::Append() {
  QuestionnaireDialog dialog(main_window_, 1);
  dialog.exec();
  dialog.show();
  dialog.setWindowModality(Qt::WindowModal);
  return;
}

void SetupDialog::CheckInput() {
  buttons_->button(QDialogButtonBox::Ok)->setEnabled(false);
  app_button_->setEnabled(false);
  if (!user_name_line_edit_->text().isEmpty())
    if (!project_title_line_edit_->text().isEmpty())
      if (!project_dir_line_edit_->text().isEmpty())
        if (!log_file_dir_line_edit_->text().isEmpty())
          if (!log_file_name_line_edit_->text().isEmpty()) {
            buttons_->button(QDialogButtonBox::Ok)->setEnabled(true);
            app_button_->setEnabled(true);
          }
  return;
}

bool SetupDialog::ProjectHandler() {
  project_dir_ = project_dir_line_edit_->text() + tr("/");
  project_dir_ += project_title_line_edit_->text() + tr(".pro");
  project_dir_ = QDir::toNativeSeparators(project_dir_);
  QFileInfo checkFile(project_dir_);
  if (checkFile.exists() && checkFile.isFile()) {
    new_project_flag_ = false;
    project_->ClearProject();
#ifdef CRYPT
    bool status = project_->Load(project_dir_);

    while (status == false) {
      PasswordDialog *pwd = new PasswordDialog(crypt_, false);
      if (pwd->exec() == QDialog::Rejected) {
        return false;
      }
      status = project_->Load(project_dir_);
      if (status == false) {
        QMessageBox box;
        box.setInformativeText("Password Incorrect.");
        box.setStandardButtons(QMessageBox::Ok);
        box.exec();
      }
    }
#else
    project_->Load(project_dir_);
#endif
    if (project_->GetLogFile() != log_file_name_line_edit_->text())
      project_->SetLogFile(log_file_name_line_edit_->text());
    if (project_->GetLogDir() != log_file_dir_line_edit_->text())
      project_->SetLogDir(log_file_dir_line_edit_->text());
    if (project_->GetProDir() != project_dir_line_edit_->text())
      project_->SetProDir(project_dir_line_edit_->text());
    if (project_->GetTitle() != project_title_line_edit_->text())
      project_->SetTitle(project_title_line_edit_->text());
  } else {
    new_project_flag_ = true;
  }
  return true;
}

void SetupDialog::AutoComplete() {
  QStringListModel *model = new QStringListModel();
  QStringList l("*.pro");
  QDir dir(QDir::toNativeSeparators(project_dir_line_edit_->text()));
  dir_list_ = dir.entryList(l);
  dir_list_.replaceInStrings(".pro", "");
  model->setStringList(dir_list_);
  completer_->setModel(model);
  project_title_line_edit_->update();
  return;
}
