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

#include "proinitdialog.h"


ProInitDialog::ProInitDialog(Project *pro) : QDialog() {
  project_ = pro;
  Setup();
  CreateConnections();
}

void ProInitDialog::accept() {
  project_->SetInitStage(stages_button_group_->checkedButton()->text());
  project_->SetStageComment(comment_box_->toPlainText());
  QDialog::accept();
}

void ProInitDialog::reject() {
  QDialog::reject();
}

void ProInitDialog::Setup() {
  QLabel *info = new QLabel;
  info->setText(tr("You received this questionnaire since it is the first time "
                   "you created this project."));
  info->setWordWrap(true);

  // Specify stage of the project
  QLabel *stages_label = new QLabel;
  stages_label->setText(tr("Please specify the current development stage of "
                           "the project. If there is no entry matching your "
                           "current project please note your current "
                           "development stage in the comment box to the "
                           "right."));
  stages_label->setWordWrap(true);
  stages_button_group_ = new QButtonGroup;
  stages_buttons_[0] = new QRadioButton(tr("Scratch"));
  stages_button_group_->addButton(stages_buttons_[0],0);
  stages_buttons_[0]->setShortcut(QKeySequence(Qt::Key_B));
  stages_buttons_[0]->setToolTip(tr("The project is developed from scratch"));
  stages_buttons_[1] = new QRadioButton(tr("Working serial version"));
  stages_button_group_->addButton(stages_buttons_[1],1);
  stages_buttons_[1]->setShortcut(QKeySequence(Qt::Key_H));
  stages_buttons_[1]->setToolTip(tr("The project's developers make use of a "
                                    "working serial version which shall be "
                                    "tuned or parallelized"));
  stages_buttons_[2] = new QRadioButton(tr("Tuned serial version"));
  stages_button_group_->addButton(stages_buttons_[2],2);
  stages_buttons_[2]->setShortcut(QKeySequence(Qt::Key_S));
  stages_buttons_[2]->setToolTip(tr("The project's developers make use of a "
                                    "tuned serial version which shall be "
                                    "parallelized"));
  stages_buttons_[3] = new QRadioButton(tr("Working parallel version"));
  stages_button_group_->addButton(stages_buttons_[3],3);
  stages_buttons_[3]->setShortcut(QKeySequence(Qt::Key_P));
  stages_buttons_[3]->setToolTip(tr("The project's developers make use of a "
                                    "working parallel version which shall be "
                                    "tuned"));
  stages_buttons_[4] = new QRadioButton(tr("Tuned parallel version"));
  stages_button_group_->addButton(stages_buttons_[4],4);
  stages_buttons_[4]->setShortcut(QKeySequence(Qt::Key_T));
  stages_buttons_[4]->setToolTip(tr("The project's developers make use of a "
                                    "tuned parallel version which shall be "
                                    "improved"));
  stages_buttons_[0]->setChecked(true);  // Set default button

  QLabel *comment_label = new QLabel;
  comment_label->setText(tr("Comments on the current development stage, the "
                            "used programming model, goals of the project, "
                            "initial performance, used architecture, etc.:"));
  comment_label->setWordWrap(true);
  comment_box_ = new QPlainTextEdit;
  comment_box_->setPlaceholderText(tr("Comment..."));

  // Separators
  QFrame* central_line = new QFrame();
  central_line->setGeometry(QRect(/* ... */));
  central_line->setFrameShape(QFrame::VLine);
  central_line->setFrameShadow(QFrame::Sunken);
  QFrame* top_line = new QFrame();
  top_line->setGeometry(QRect(/* ... */));
  top_line->setFrameShape(QFrame::HLine);
  top_line->setFrameShadow(QFrame::Sunken);
  QFrame* botton_line = new QFrame();
  botton_line->setGeometry(QRect(/* ... */));
  botton_line->setFrameShape(QFrame::HLine);
  botton_line->setFrameShadow(QFrame::Sunken);

  buttons_ = new QDialogButtonBox(QDialogButtonBox::Ok
                                  | QDialogButtonBox::Cancel, Qt::Horizontal);

  // Layout
  QVBoxLayout *stages_layout = new QVBoxLayout;
  for (int i = 0; i < 5; i++) {
    stages_layout->addWidget(stages_buttons_[i]);
  }
  stages_layout->addStretch(1);

  QVBoxLayout *l_layout = new QVBoxLayout;
  l_layout->addWidget(stages_label);
  l_layout->addLayout(stages_layout);

  QVBoxLayout *r_layout = new QVBoxLayout;
  r_layout->addWidget(comment_label);
  r_layout->addWidget(comment_box_);

  QHBoxLayout *colomn_layout = new QHBoxLayout;
  colomn_layout->addLayout(l_layout);
  colomn_layout->addWidget(central_line);
  colomn_layout->addLayout(r_layout);

  QVBoxLayout *main_layout = new QVBoxLayout;
  main_layout->addWidget(info);
  main_layout->addWidget(top_line);
  main_layout->addLayout(colomn_layout);
  main_layout->addWidget(botton_line);
  main_layout->addWidget(buttons_);

  setLayout(main_layout);
  setWindowTitle(tr("Inital project stage"));
}

void ProInitDialog::CreateConnections() {
  connect(buttons_, SIGNAL(accepted()), this, SLOT(accept()));
  connect(buttons_, SIGNAL(rejected()), this, SLOT(reject()));
}
