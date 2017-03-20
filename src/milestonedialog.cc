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

#include "milestonedialog.h"

#include <QDebug>
#include <QMessageBox>

MilestoneDialog::MilestoneDialog(Milestone *m, QString msg) : QDialog() {
  ms_ = m;
  msg_ = msg;
  Setup();
  CreateConnections();
  LoadPreviousMS();
}

void MilestoneDialog::accept() {
  ms_->SetComment(comment_->text());
  ms_->SetArc(arc_comment_->text());
  ms_->SetModel(model_comment_->text());
  ms_->SetPerfMetric(perf_box_->currentText());
  if (perf_box_->currentIndex() == 5) {
    ms_->SetPerfComment(perf_comment_->text());
  } else {
    ms_->SetPerfComment(perf_spin_->text());
  }
  ms_->SetCompiler(compiler_comment_->text());
  ms_->SetThreadsType(threads_box_->currentText());
  if (threads_box_->currentIndex() == 2) {
    ms_->SetThreadsComment(threads_comment_->text());
  } else {
    ms_->SetThreadsComment(threads_spin_->text());
  }
  ms_->SetDataSize(data_size_->text());
  QDialog::accept();
}

void MilestoneDialog::reject() {
  QMessageBox::StandardButton button;
  button = QMessageBox::question(this, msg_,tr("Are you sure you want to "
                                                    "skip this log?\n"),
                                 QMessageBox::No | QMessageBox::Yes,
                                 QMessageBox::Yes);
  if (button != QMessageBox::No) {
    QDialog::reject();
  }
}

void MilestoneDialog::Setup() {
  QLabel *general_label = new QLabel(tr("Please specify the following "
                                        "information regarding this "
                                        "milestone:"));
  // Performance
  QLabel *perf_label = new QLabel(tr("1. Performance"));
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
  perf_spin_->setFocus();
  perf_comment_ = new QLineEdit;
  perf_comment_->setPlaceholderText(tr("Specify performance"));
  perf_comment_->hide();
  perf_box_->setCurrentIndex(0);
  perf_spin_->setSuffix(tr(" sec"));

  // Architecture
  QLabel *arc_label = new QLabel(tr("2. Architecture"));
  arc_label->setWordWrap(true);
  arc_comment_ = new QLineEdit;
  arc_comment_->setToolTip(tr("Information on the used architecture for the "
                              "performance measurements"));
  arc_comment_->setPlaceholderText(tr("E.g. Intel Xeon X7550"));

  // No threads, nodes, etc.
  QLabel *threads_label = new QLabel(tr("3. No threads, nodes, etc."));
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

  // Compiler
  QLabel *compiler_label = new QLabel(tr("4. Compiler"));
  compiler_label->setWordWrap(true);
  compiler_comment_ = new QLineEdit;
  compiler_comment_->setToolTip(tr("Information on used compiler for the "
                                   "performance measurements"));
  compiler_comment_->setPlaceholderText(tr("E.g. Intel Compiler 14.0"));

  // Programming model
  QLabel *model_label = new QLabel(tr("5. Programming model"));
  model_label->setWordWrap(true);
  model_comment_ = new QLineEdit;
  model_comment_->setToolTip(tr("Information on the programming model used "
                                "during this milestone"));
  model_comment_->setPlaceholderText(tr("E.g. OpenMP"));

  // Data size
  QLabel *data_size_label = new QLabel(tr("6. Data size"));
  data_size_label->setWordWrap(true);
  data_size_ = new QLineEdit;
  data_size_->setToolTip(tr("Information on the size of the used data set"));
  data_size_->setPlaceholderText(tr("E.g. changes to the data size"));

  // General comments
  QLabel *comment_label = new QLabel(tr("7. Other comments"));
  comment_label->setWordWrap(true);
  comment_ = new QLineEdit;
  comment_->setToolTip(tr("Any other comment not fitting the categories above"));
  comment_->setPlaceholderText(tr("General comment on this milestone"));

  // Button to skip the input
  skip_button_ = new QPushButton(tr("Skip"));
  skip_button_->setToolTip(tr("Skip this milestone <font color='gray'>Q</font>"));
  skip_button_->setCheckable(true);
  skip_button_->setAutoDefault(false);
  skip_button_->setShortcut(QKeySequence(Qt::Key_Q));

  // Button to finish the input
  finish_button_ = new QPushButton(tr("Finish"));
  finish_button_->setToolTip(tr("Finish input <font color='gray'>F</font>"));
  finish_button_->setCheckable(true);
  finish_button_->setAutoDefault(false);
  finish_button_->setShortcut(QKeySequence(Qt::Key_F));
  finish_button_->setEnabled(false);

  // Separators
  QFrame *sep_line[8];
  for (int i = 0; i < 8; i++) {
    sep_line[i] = new QFrame();
    sep_line[i]->setGeometry(QRect(/* ... */));
    sep_line[i]->setFrameShape(QFrame::HLine);
    sep_line[i]->setFrameShadow(QFrame::Sunken);
  }

  // Layout
  QHBoxLayout *button_layout = new QHBoxLayout;
  button_layout->addWidget(skip_button_, 0, Qt::AlignCenter);
  button_layout->addWidget(finish_button_, 0, Qt::AlignCenter);

  main_layout_ = new QVBoxLayout;
  main_layout_->addWidget(general_label);
  main_layout_->addWidget(sep_line[0]);
  main_layout_->addWidget(perf_label);
  main_layout_->addWidget(perf_box_);
  main_layout_->addWidget(perf_spin_);
  main_layout_->addWidget(perf_comment_);
  main_layout_->addWidget(sep_line[1]);
  main_layout_->addWidget(arc_label);
  main_layout_->addWidget(arc_comment_);
  main_layout_->addWidget(sep_line[2]);
  main_layout_->addWidget(threads_label);
  main_layout_->addWidget(threads_box_);
  main_layout_->addWidget(threads_spin_);
  main_layout_->addWidget(threads_comment_);
  main_layout_->addWidget(sep_line[3]);
  main_layout_->addWidget(compiler_label);
  main_layout_->addWidget(compiler_comment_);
  main_layout_->addWidget(sep_line[4]);
  main_layout_->addWidget(model_label);
  main_layout_->addWidget(model_comment_);
  main_layout_->addWidget(sep_line[5]);
  main_layout_->addWidget(data_size_label);
  main_layout_->addWidget(data_size_);
  main_layout_->addWidget(sep_line[6]);
  main_layout_->addWidget(comment_label);
  main_layout_->addWidget(comment_);
  main_layout_->addWidget(sep_line[7]);
  main_layout_->addLayout(button_layout);

  setLayout(main_layout_);
  setWindowTitle("Milestone: " + msg_);
}

void MilestoneDialog::CreateConnections() {
  connect(perf_box_, SIGNAL(activated(int)), this,
          SLOT(PerfInputChanged(int)));
  connect(threads_box_, SIGNAL(activated(int)), this,
          SLOT(ThreadsInputChanged(int)));

  connect(perf_box_, SIGNAL(activated(int)), this, SLOT(CheckInput()));
  connect(perf_comment_, SIGNAL(textChanged(QString)), this,
          SLOT(CheckInput()));
  connect(arc_comment_, SIGNAL(textChanged(QString)), this, SLOT(CheckInput()));
  connect(threads_box_, SIGNAL(activated(int)), this, SLOT(CheckInput()));
  connect(threads_comment_, SIGNAL(textChanged(QString)), this,
          SLOT(CheckInput()));
  connect(compiler_comment_, SIGNAL(textChanged(QString)), this,
          SLOT(CheckInput()));
  connect(model_comment_, SIGNAL(textChanged(QString)), this,
          SLOT(CheckInput()));

  connect(skip_button_, SIGNAL(released()), this, SLOT(reject()));
  connect(finish_button_, SIGNAL(released()), this, SLOT(accept()));
}

void MilestoneDialog::PerfInputChanged(int i) {
  switch(i) {
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
}

void MilestoneDialog::ThreadsInputChanged(int i) {
  switch(i) {
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
}

void MilestoneDialog::CheckInput() {
  finish_button_->setEnabled(false);
  bool is_empty[5] = {false};
  if (perf_box_->currentIndex() == 5) {
    is_empty[0] = perf_comment_->text().isEmpty();
  } else {
    is_empty[0] = false;
  }
  is_empty[1] = arc_comment_->text().isEmpty();
  if (threads_box_->currentIndex() == 2) {
    is_empty[2] = threads_comment_->text().isEmpty();
  } else {
    is_empty[1] = false;
  }
  is_empty[3] = compiler_comment_->text().isEmpty();
  is_empty[4] = model_comment_->text().isEmpty();

  if (std::accumulate(is_empty, is_empty + 5, 0) == 0) {
    finish_button_->setEnabled(true);
  }
}

void MilestoneDialog::LoadPreviousMS() {
  if (ms_->GetMsId() == -1) {
    QString perf = ms_->GetPerfMetric();
    if (perf.contains("Execution time in seconds", Qt::CaseInsensitive)) {
      perf_box_->setCurrentIndex(0);
      PerfInputChanged(0);
    } else if (perf.contains("Execution time in minutes", Qt::CaseInsensitive)) {
      perf_box_->setCurrentIndex(1);
      PerfInputChanged(1);
    } else if (perf.contains("Execution time in hours", Qt::CaseInsensitive)) {
      perf_box_->setCurrentIndex(2);
      PerfInputChanged(2);
    } else if (perf.contains("Throughput in GFlop/s", Qt::CaseInsensitive)) {
      perf_box_->setCurrentIndex(3);
      PerfInputChanged(3);
    } else if (perf.contains("Speedup", Qt::CaseInsensitive)) {
      perf_box_->setCurrentIndex(4);
      PerfInputChanged(4);
    } else if (perf.contains("Other", Qt::CaseInsensitive)) {
      perf_box_->setCurrentIndex(5);
      PerfInputChanged(5);
    }
    QString threads = ms_->GetThreadsType();
    if (threads.contains("Number of threads", Qt::CaseInsensitive)) {
      threads_box_->setCurrentIndex(0);
      ThreadsInputChanged(0);
    } else if (threads.contains("Number of nodes", Qt::CaseInsensitive)) {
      threads_box_->setCurrentIndex(1);
      ThreadsInputChanged(1);
    } else if (threads.contains("Other", Qt::CaseInsensitive)) {
      threads_box_->setCurrentIndex(2);
      ThreadsInputChanged(2);
    }
    compiler_comment_->setText(ms_->GetCompiler());
    model_comment_->setText(ms_->GetModel());
    arc_comment_->setText(ms_->GetArc());
    data_size_->setText(ms_->GetDataSize());
  }
}
