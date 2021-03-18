/*
 * Copyright (c) 2015-2019 by IT Center, RWTH Aachen University
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

#include "qstform.h"

QstForm::QstForm() : QWidget() { Setup(); }

QstForm::QstForm(QString title, QString tip) : QWidget() {
  tooltip_ = tip;
  Setup();
  title_->setText(title);
  line_edit_->setToolTip(tooltip_);
}

QstForm::QstForm(QString title, QString tip, bool line) : QWidget() {
  tooltip_ = tip;
  line_ = line;
  Setup();
  title_->setText(title);
  line_edit_->setToolTip(tooltip_);
}

void QstForm::Setup() {
  title_ = new QLabel();
  title_->setWordWrap(true);

  line_edit_ = new QLineEdit;

  SetCompleter();

  layout_ = new QVBoxLayout;
  layout_->addWidget(title_);
  layout_->addWidget(line_edit_);
  if (line_) {
    hline_ = new QFrame();
    hline_->setGeometry(QRect(/* ... */));
    hline_->setFrameShape(QFrame::HLine);
    hline_->setFrameShadow(QFrame::Sunken);
    layout_->addWidget(hline_);
  }
  layout_->setContentsMargins(0, 0, 0, 0);
  setLayout(layout_);
}

void QstForm::SetTitle(QString t) { title_->setText(t); }

QString QstForm::GetTitle() const { return title_->text(); }

void QstForm::SetToolTip(QString t) { line_edit_->setToolTip(t); }

QString QstForm::GetToolTip() const { return line_edit_->toolTip(); }

void QstForm::SetText(QString t) { line_edit_->setText(t); }

QString QstForm::GetText() const { return line_edit_->text(); }

void QstForm::SetPlaceholderText(QString t) {
  line_edit_->setPlaceholderText(t);
}

QString QstForm::GetPlaceholderText() const {
  return line_edit_->placeholderText();
}

void QstForm::SetLine(bool l) {
  line_ = l;
  if (l) {
    if (!layout_->findChild<QWidget *>("hline_")) {
      hline_ = new QFrame();
      hline_->setGeometry(QRect(/* ... */));
      hline_->setFrameShape(QFrame::HLine);
      hline_->setFrameShadow(QFrame::Sunken);
      layout_->addWidget(hline_);
    }
  } else {
    layout_->removeWidget(hline_);
  }
}

bool QstForm::GetLine() const { return line_; }

void QstForm::SetCompleter() {
  completer_ = new QCompleter(compContents_, this);
  completer_->setCaseSensitivity(Qt::CaseInsensitive);
  completer_->setCompletionMode(QCompleter::PopupCompletion);
  line_edit_->setCompleter(completer_);
}

void QstForm::SetCompleter(QStringList s) {
  completer_ = new QCompleter(s, this);
  completer_->setCaseSensitivity(Qt::CaseInsensitive);
  completer_->setCompletionMode(QCompleter::PopupCompletion);
  line_edit_->setCompleter(completer_);
}

QStringList QstForm::GetCompleter() const { return compContents_; }

void QstForm::CompleterAppend(QString s) {
  compContents_.append(s);
  SetCompleter();
}
