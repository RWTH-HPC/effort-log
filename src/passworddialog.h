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

#ifndef PASSWORDDIALOG_H
#define PASSWORDDIALOG_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

#include "crypt.h"


//! Class to handle password input
/*!
 * This class provides a dialog which lets users set up new passwords and
 * enter already set passwords.
 */
class PasswordDialog : public QDialog {
  Q_OBJECT
public:
  //! The main constructor.
  /*!
   * \param[in] crypt A pointer to Crypt which is used to store the new
   * password or holds the correct password
   *  \param[in] init Indicates if it is a new password or not
   */
  PasswordDialog(Crypt *crypt, bool init);
private slots:
  //! Check if passwords match and are non-empty
  void OnPwdInput();
private:
  //! Overrides the default accept() member.
  /*!
   * accept() is executed on successfully quitting the dialog. This method sets
   * the new password if the mode is set to new.
   */
  void accept() Q_DECL_OVERRIDE;
  //! Initializes a new password
  void PwdInit();
  //! Handles user input of passwords for existing projects
  void PwdEnter();

  QDialogButtonBox *buttons_;  /**< Buttons OK and CLOSE at the buttom of the
                                    dialog.*/
  QLineEdit *pwd_prompt_;  /**< Lets user input their password.*/
  QLineEdit *rep_pwd_prompt_;  /**< Lets user repeate their password.*/
  Crypt *crypt_;  /**< A pointer to Crypt which is used to store the new
                       password or holds the correct password.*/
  QLabel *match_label_;  /**< Displays a warning if passwords do not match or
                              are empty.*/
};

#endif // PASSWORDDIALOG_H
