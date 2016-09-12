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
  setWindowTitle("");

  int no_ms = pro->GetNoMilestones();
  int no_events = pro->GetNoActivities();
  QStandardItemModel *model = new QStandardItemModel(0, 6, this);
  QStandardItem *rootNode = model->invisibleRootItem();
  QList<QDate> date_list;
  QStandardItem *current;
  for (int i = 0; i < no_events; i++) {
    QList<QStandardItem *> item_list;
    QStandardItem *item = new QStandardItem();
    item_list << item;
    QStandardItem *item_id = new QStandardItem();
    item_id->setText(QString::number(i + 1));
    item_id->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    item_list << item_id;
    item_list << new QStandardItem(pro->GetActivity(i).GetComment());
    item_list << new QStandardItem(pro->GetActivity(i).GetType());
    QStandardItem *item_interval = new QStandardItem();
    item_interval->setText(
        QString::number(pro->GetActivity(i).GetIntervalTime()));
    item_interval->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    item_list << item_interval;

    QDate date = pro->GetActivity(i).GetCurTime().date();
    if (date_list.contains(date) == false) {
      date_list.append(date);
      QStandardItem *item = new QStandardItem(date.toString("MM-dd-yyyy"));
      rootNode->appendRow(item);
      current = item;
    }

    int no_ms_matches = 0;
    for (int j = 0; j < no_ms; j++) {
      if (pro->GetMilestone(j).GetEventId() == pro->GetActivity(i).GetId()) {
        QString ms_content;
        QStandardItem *item_ms = new QStandardItem();
        ms_content.append(pro->GetMilestone(j).GetTitle());
        ms_content.append(", ");
        QString metric = pro->GetMilestone(j).GetPerfMetric();
        if (metric.contains("Execution time")) {
          ms_content.append("Exec. time: ");
        } else if (metric.contains("Throughput")) {
          ms_content.append("Throughput: ");
        } else {
          ms_content.append(metric);
          ms_content.append(": ");
        }
        ms_content.append(pro->GetMilestone(j).GetPerfComment());
        item_ms->setText(ms_content);
        if (no_ms_matches == 0) {
          item_list << item_ms;
          current->appendRow(item_list);
        } else {
          QList<QStandardItem *> ms_item_list;
          ms_item_list << new QStandardItem();
          for (int n = 0; n < 3; n++) {
            QStandardItem *item_ditto = new QStandardItem("\"");
            item_ditto->setTextAlignment(Qt::AlignCenter | Qt::AlignVCenter);
            ms_item_list << item_ditto;
          }
          QStandardItem *item_ditto = new QStandardItem("\"");
          item_ditto->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
          ms_item_list << item_ditto;
          ms_item_list << item_ms;
          current->appendRow(ms_item_list);
        }
        no_ms_matches++;
      }
    }
    if (no_ms_matches == 0)
      current->appendRow(item_list);
  }
  model->setHeaderData(0, Qt::Horizontal, QObject::tr("Date"));
  model->setHeaderData(1, Qt::Horizontal, QObject::tr("Id"));
  model->setHeaderData(2, Qt::Horizontal, QObject::tr("Comment"));
  model->setHeaderData(3, Qt::Horizontal, QObject::tr("Activity"));
  model->setHeaderData(4, Qt::Horizontal, QObject::tr("Duration [min]"));
  model->setHeaderData(5, Qt::Horizontal, QObject::tr("Milestone"));

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
}
