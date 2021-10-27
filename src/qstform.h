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

#pragma once

#include <QCompleter>
#include <QLabel>
#include <QLineEdit>
#include <QStringListModel>
#include <QVBoxLayout>
#include <QWidget>

//! Class to handle projects.
/*!
 * This class holds all information to specify a project.
 * Additionally there is an interface to load from a file in JSON format or
 * save a list of activities to such file.
 */
class QstForm : public QWidget {
  Q_OBJECT
 public:
  //! The main constructor.
  /*!
   * \param[in] parent The parent widget
   */
  explicit QstForm();
  //! The main constructor.
  /*!
   * \param[in] parent The parent widget
   * \param[in] title The title of the form
   * \param[in] tip The tooltip of the form
   */
  explicit QstForm(QString title, QString tip);
  //! The main constructor.
  /*!
   * \param[in] parent The parent widget
   * \param[in] title The title of the form
   * \param[in] tip The tooltip of the form
   * \param[in] line Sets if there will be a horizontal line printed after the
   * form
   */
  explicit QstForm(QString title, QString tip, bool line);
  //! Setter for title_
  /*!
   * \param[in] t The title of the form
   */
  void SetTitle(QString t);
  //! Getter for title_
  /*!
   * \return A string containing title_
   */
  QString GetTitle() const;
  //! Setter for tooltip_
  /*!
   * \param[in] t The tooltip of the form
   */
  void SetToolTip(QString t);
  //! Getter for tooltip_
  /*!
   * \return A string containing tooltip_
   */
  QString GetToolTip() const;
  //! Setter for line edit text
  /*!
   * \param[in] t The text to set for line_edit_
   */
  void SetText(QString t);
  //! Getter for the text of line_edit_
  /*!
   * \return A string containing line_edit_->text()
   */
  QString GetText() const;
  //! Setter for line edit text
  /*!
   * \param[in] t The placeholder text to set for line_edit_
   */
  void SetPlaceholderText(QString t);
  //! Getter for the placeholder text of line_edit_
  /*!
   * \return A string containing the placeholder text of line_edit_
   */
  QString GetPlaceholderText() const;
  //! Setter for line_
  /*!
   * \param[in] l Sets if there will be a horizontal line printed after the form
   */
  void SetLine(bool s);
  //! Getter for line_
  /*!
   * \return A bool indicating if there will be a horizontal line printed after
   * the form
   */
  bool GetLine() const;
  //! Setter for completer_
  /*!
   * \param[in] s A QStringList to be set for completer_
   */
  void SetCompleter();
  //! Setter for completer_
  /*!
   * \param[in] s A QStringList to be set for completer_
   */
  void SetCompleter(QStringList s);
  //! Getter for completer_
  /*!
   * \return A QStringList containing the current completer model
   */
  QStringList GetCompleter() const;
  //! Append a string to the completer
  /*!
   * \param[in] s The QString to append
   */
  void CompleterAppend(QString s);
  QLineEdit *line_edit_; /**< The main text input field.*/

 private:
  //! Sets up the form
  /*!
   * This method initilaizes all objects needed for the form.
   */
  void Setup();
  QLabel *title_;            /**< The title of the form.*/
  QString tooltip_;          /**< The tooltip of the form.*/
  QVBoxLayout *layout_;      /**< The layout of the form.*/
  QCompleter *completer_;    /**< The completer for the input field.*/
  QStringList compContents_; /**< The completer's content.*/
  bool line_ = true; /**< Sets if there will be a horizontal line printed after
                        the form.*/
  QFrame *hline_;    /**< A horizontal line printed after the form.*/
};
