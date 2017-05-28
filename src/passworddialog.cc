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

#include <QDebug>

#include "passworddialog.h"

PasswordDialog::PasswordDialog(Crypt *crypt, bool init) : QDialog() {
  crypt_ = crypt;
  if (init == true) {
    PwdInit();
  } else {
    PwdEnter();
  }
}

void PasswordDialog::PwdInit() {
  QLabel *pwd_prompt_label = new QLabel("Please set a password which is used "
                                        "to encrypt your private log files.");
  pwd_prompt_label->setWordWrap(true);
  pwd_prompt_ = new QLineEdit;
  pwd_prompt_->setEchoMode(QLineEdit::Password);
  pwd_prompt_->setPlaceholderText(tr("Password"));
  rep_pwd_prompt_ = new QLineEdit;
  rep_pwd_prompt_->setEchoMode(QLineEdit::Password);
  rep_pwd_prompt_->setPlaceholderText(tr("Repeat Password"));
  match_label_ = new QLabel("Password too short");

  buttons_ = new QDialogButtonBox(
      QDialogButtonBox::Ok | QDialogButtonBox::Close, Qt::Horizontal);
  buttons_->button(QDialogButtonBox::Ok)->setEnabled(false);

  QVBoxLayout *main_layout = new QVBoxLayout;
  main_layout->addWidget(pwd_prompt_label);
  main_layout->addWidget(pwd_prompt_);
  main_layout->addWidget(rep_pwd_prompt_);
  main_layout->addWidget(buttons_);
  main_layout->addWidget(match_label_);
  setLayout(main_layout);
  setWindowTitle("Password prompt");

  // Connections
  connect(pwd_prompt_, SIGNAL(textChanged(QString)), this, SLOT(OnPwdInput()));
  connect(rep_pwd_prompt_, SIGNAL(textChanged(QString)), this,
          SLOT(OnPwdInput()));
  connect(buttons_, SIGNAL(accepted()), this, SLOT(accept()));
  connect(buttons_, SIGNAL(rejected()), this, SLOT(reject()));
  return;
}

void PasswordDialog::PwdEnter() {
  QLabel *pwd_prompt_label = new QLabel("Please enter the password for this "
                                        "project.");
  pwd_prompt_label->setWordWrap(true);
  pwd_prompt_ = new QLineEdit;
  pwd_prompt_->setEchoMode(QLineEdit::Password);
  pwd_prompt_->setPlaceholderText(tr("Password"));

  buttons_ = new QDialogButtonBox(
      QDialogButtonBox::Ok | QDialogButtonBox::Close, Qt::Horizontal);

  QVBoxLayout *main_layout = new QVBoxLayout;
  main_layout->addWidget(pwd_prompt_label);
  main_layout->addWidget(pwd_prompt_);
  main_layout->addWidget(buttons_);
  setLayout(main_layout);
  setWindowTitle("Password prompt");

  // Connections
  connect(buttons_, SIGNAL(accepted()), this, SLOT(accept()));
  connect(buttons_, SIGNAL(rejected()), this, SLOT(reject()));
  return;
}

void PasswordDialog::OnPwdInput() {
  buttons_->button(QDialogButtonBox::Ok)->setEnabled(false);
  if (!pwd_prompt_->text().isEmpty()) {
    match_label_->setText(tr("Password is empty"));
    if (!rep_pwd_prompt_->text().isEmpty()) {
      match_label_->setText(tr("Password is empty"));
      if (pwd_prompt_->text() == rep_pwd_prompt_->text()) {
        buttons_->button(QDialogButtonBox::Ok)->setEnabled(true);
        match_label_->setText(tr(""));
      } else {
        match_label_->setText(tr("Passwords do not match"));
      }
    }
  }
  return;
}

void PasswordDialog::accept() {
  QByteArray pwd;
  pwd.append(pwd_prompt_->text());
  crypt_->SetPwd(pwd);
  QDialog::accept();
}
