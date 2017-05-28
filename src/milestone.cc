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

#include "milestone.h"

Milestone::Milestone() {}

void Milestone::Clear() {
  milestone_id_ = 0;
  event_id_ = 0;
  title_ = "";
  general_comment_ = "";
  arc_ = "";
  model_ = "";
}

void Milestone::SetMsId(int id) { milestone_id_ = id; }

int Milestone::GetMsId() const { return milestone_id_; }

void Milestone::SetEventId(int id) { event_id_ = id; }

int Milestone::GetEventId() const { return event_id_; }

void Milestone::SetTitle(QString t) { title_ = t; }

QString Milestone::GetTitle() const { return title_; }

void Milestone::SetTime(QDateTime t) { time_ = t; }

QDateTime Milestone::GetTime() const { return time_; }

void Milestone::SetComment(QString c) { general_comment_ = c; }

QString Milestone::GetComment() const { return general_comment_; }

void Milestone::SetPerfMetric(QString m) { perf_metric_ = m; }

QString Milestone::GetPerfMetric() const { return perf_metric_; }

void Milestone::SetPerfComment(QString c) { perf_comment_ = c; }

QString Milestone::GetPerfComment() const { return perf_comment_; }

void Milestone::SetArc(QString a) { arc_ = a; }

QString Milestone::GetArc() const { return arc_; }

void Milestone::SetThreadsType(QString t) { threads_type_ = t; }

QString Milestone::GetThreadsType() const { return threads_type_; }

void Milestone::SetThreadsComment(QString c) { threads_comment_ = c; }

QString Milestone::GetThreadsComment() const { return threads_comment_; }

void Milestone::SetCompiler(QString c) { compiler_ = c; }

QString Milestone::GetCompiler() const { return compiler_; }

void Milestone::SetModel(QString m) { model_ = m; }

QString Milestone::GetModel() const { return model_; }

void Milestone::SetDataSize(QString d) { data_size_ = d; }

QString Milestone::GetDataSize() const { return data_size_; }
