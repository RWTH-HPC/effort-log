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

#include "logview.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QPushButton>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QTreeView>
#include <QVBoxLayout>

LogView::LogView(Project *pro) : QDialog() {
  QTreeView *view;
  int no_events = pro->GetNoActivities();
  QStandardItemModel *model = new QStandardItemModel(0, 7, this);
  QStandardItem *rootNode = model->invisibleRootItem();
  QList<QDate> date_list;
  QStandardItem *current = new QStandardItem;
  for (int i = 0; i < no_events; i++) {
    QList<QStandardItem *> item_list;
    QStandardItem *item = new QStandardItem();
    item_list << item;
    QStandardItem *item_id = new QStandardItem();
    item_id->setText(QString::number(i + 1));
    item_id->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    item_list << item_id;
    item_list << new QStandardItem(pro->GetActivity(i).GetType());
    QStandardItem *item_interval = new QStandardItem();
    item_interval->setText(
        QString::number(pro->GetActivity(i).GetIntervalTime()));
    item_interval->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    item_list << item_interval;

    QStandardItem *item_perf = new QStandardItem();
    if (pro->GetActivity(i).GetPerfComment() != "") {
      QString ms_content;
      QString metric = pro->GetActivity(i).GetPerfMetric();
      if (metric != "") {
        if (metric.contains("Execution time")) {
          ms_content.append("Exec. time: ");
        } else if (metric.contains("Throughput")) {
          ms_content.append("Throughput: ");
        } else if (metric.contains("Speedup")) {
          ms_content.append("Speedup: ");
        } else {
          ms_content.append(metric);
          ms_content.append(": ");
        }
      }
      ms_content.append(pro->GetActivity(i).GetPerfComment());
      item_perf->setText(ms_content);
    }
    item_list << item_perf;
    item_list << new QStandardItem(pro->GetActivity(i).GetMsTitle());
    item_list << new QStandardItem(pro->GetActivity(i).GetComment());

    QDate date = pro->GetActivity(i).GetCurTime().date();
    if (date_list.contains(date) == false) {
      date_list.append(date);
      QStandardItem *item = new QStandardItem(date.toString("MM-dd-yyyy"));
      rootNode->appendRow(item);
      current = item;
    }
    current->appendRow(item_list);
  }

  model->setHeaderData(0, Qt::Horizontal, QObject::tr("Date"));
  model->setHeaderData(1, Qt::Horizontal, QObject::tr("Id"));
  model->setHeaderData(2, Qt::Horizontal, QObject::tr("Activity"));
  model->setHeaderData(3, Qt::Horizontal, QObject::tr("Duration [min]"));
  model->setHeaderData(4, Qt::Horizontal, QObject::tr("Performance"));
  model->setHeaderData(5, Qt::Horizontal, QObject::tr("Milestone"));
  model->setHeaderData(6, Qt::Horizontal, QObject::tr("Comment"));

  view = new QTreeView(this);
  view->setModel(model);
  view->setEditTriggers(QAbstractItemView::NoEditTriggers);
  view->setColumnWidth(0, 110);
  view->resizeColumnToContents(1);
  view->setColumnWidth(3, 100);
  view->resizeColumnToContents(4);

  QPushButton *closeBtn = new QPushButton(tr("Close"));
  connect(closeBtn, SIGNAL(clicked()), this, SLOT(accept()));

  QVBoxLayout *layout = new QVBoxLayout;
  QHBoxLayout *bottomLayout = new QHBoxLayout;
  bottomLayout->addStretch();
  bottomLayout->addWidget(closeBtn);

  layout->addWidget(view);
  layout->addLayout(bottomLayout);

  setLayout(layout);
  setWindowTitle("Viewing " + QString::number(no_events) + " events and " +
                 QString::number(pro->GetNoMilestones()) + " milestones");
  setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint |
                 Qt::WindowCloseButtonHint);
}
