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

#include "activity.h"

#include <QDebug>

Activity::Activity() {
  type_ = "";
  id_ = -1;
  saved_events_ = -1;
  project_title_ = "";
  user_name_ = "";
  log_interval_ = -1;
  cur_time_ = QDateTime();
  last_time_ = QDateTime();
  interval_time_ = -1;
  comment_ = "";
  scheduler_ = -1;
  milestone_id_ = -1;
  ms_title_ = "";
  ms_comment_ = "";
  arc_ = "";
  model_ = "";
}

const QString Activity::kActivityType[] = {
    "Break",     "Thinking", "Serial",        "Parallelizing", "Testing",
    "Debugging", "Tuning",   "Experimenting", "Other"};

QString Activity::GetType() const { return type_; }

void Activity::SetType(QString type) { type_ = type; }

void Activity::SetId(int i) { id_ = i; }

int Activity::GetId() const { return id_; }

void Activity::SetComment(QString c) { comment_ = c; }

QString Activity::GetComment() const { return comment_; }

int Activity::GetSavedEvents() const { return saved_events_; }

void Activity::SetSavedEvents(const int &events) { saved_events_ = events; }

QString Activity::GetProjectTitle() const { return project_title_; }
void Activity::SetProjectTitle(const QString &title) { project_title_ = title; }

QString Activity::GetUserName() const { return user_name_; }
void Activity::SetUserName(const QString &name) { user_name_ = name; }

int Activity::GetLogInterval() const { return log_interval_; }

void Activity::SetLogInterval(const int &interval) { log_interval_ = interval; }

QDateTime Activity::GetCurTime() const { return cur_time_; }

void Activity::SetCurTime(const QDateTime &time) { cur_time_ = time; }

QDateTime Activity::GetLastTime() const { return last_time_; }

void Activity::SetLastTime(const QDateTime &time) { last_time_ = time; }

int Activity::GetIntervalTime() const { return interval_time_; }

void Activity::SetIntervalTime(const int &time) { interval_time_ = time; }

int Activity::GetScheduler() const { return scheduler_; }
void Activity::SetScheduler(const int scheduler) { scheduler_ = scheduler; }
void Activity::SetMsId(int id) { milestone_id_ = id; }

int Activity::GetMsId() const { return milestone_id_; }

void Activity::SetMsTitle(QString t) { ms_title_ = t; }

QString Activity::GetMsTitle() const { return ms_title_; }

void Activity::SetMsComment(QString c) { ms_comment_ = c; }

QString Activity::GetMsComment() const { return ms_comment_; }

void Activity::SetPerfMetric(QString m) { perf_metric_ = m; }

QString Activity::GetPerfMetric() const { return perf_metric_; }

void Activity::SetPerfComment(QString c) { perf_comment_ = c; }

QString Activity::GetPerfComment() const { return perf_comment_; }

void Activity::SetArc(QString a) { arc_ = a; }

QString Activity::GetArc() const { return arc_; }

void Activity::SetThreadsType(QString t) { threads_type_ = t; }

QString Activity::GetThreadsType() const { return threads_type_; }

void Activity::SetThreadsComment(QString c) { threads_comment_ = c; }

QString Activity::GetThreadsComment() const { return threads_comment_; }

void Activity::SetCompiler(QString c) { compiler_ = c; }

QString Activity::GetCompiler() const { return compiler_; }

void Activity::SetModel(QString m) { model_ = m; }

QString Activity::GetModel() const { return model_; }

void Activity::SetDataSize(QString d) { data_size_ = d; }

QString Activity::GetDataSize() const { return data_size_; }
