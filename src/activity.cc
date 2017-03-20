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

}

const QString Activity::kActivityType[] = {"Break","Thinking","Serial",
                                           "Parallelizing","Testing",
                                           "Debugging","Tuning",
                                           "Experimenting","Other"};

QString Activity::GetType() const {
  return type_;
}

void Activity::SetType(QString type) {
  type_ = type;
}

void Activity::SetId(int i) {
  id_ = i;
}

int Activity::GetId() const {
  return id_;
}

int Activity::GetSavedEvents() const {
  return saved_events_;
}

void Activity::SetSavedEvents(const int &events) {
  saved_events_ = events;
}

QString Activity::GetProjectTitle() const {
  return project_title_;
}
void Activity::SetProjectTitle(const QString &title) {
  project_title_ = title;
}

QString Activity::GetUserName() const {
  return user_name_;
}
void Activity::SetUserName(const QString &name) {
  user_name_ = name;
}

int Activity::GetLogInterval() const {
  return log_interval_;
}

void Activity::SetLogInterval(const int &interval) {
  log_interval_ = interval;
}

QDateTime Activity::GetCurTime() const {
  return cur_time_;
}

void Activity::SetCurTime(const QDateTime &time) {
  cur_time_ = time;
}

QDateTime Activity::GetLastTime() const {
  return last_time_;
}

void Activity::SetLastTime(const QDateTime &time) {
  last_time_ = time;
}

int Activity::GetIntervalTime() const {
  return interval_time_;
}

void Activity::SetIntervalTime(const int &time) {
  interval_time_ = time;
}

QString Activity::GetComment() const {
  return comment_;
}
void Activity::SetComment(const QString &comment) {
  comment_ = comment;
}

int Activity::GetScheduler() const {
  return scheduler_;
}
void Activity::SetScheduler(const int scheduler) {
  scheduler_ = scheduler;
}
