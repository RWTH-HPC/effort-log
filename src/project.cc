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

#include "project.h"

#include <QDebug>

Project::Project() {
  title_ = "";
  pro_dir_ = "";
  log_dir_ = "";
  log_file_ = "";
  stage_ = "";
  stage_comment_ = "";
  interval_ = 0;
  activities_ = new QList<Activity>;
  milestones_ = new QList<Milestone>;
}

Project::Project(QString t) {
  title_ = t;
  pro_dir_ = "";
  log_dir_ = "";
  log_file_ = "";
  stage_ = "";
  stage_comment_ = "";
  interval_ = 0;
  activities_ = new QList<Activity>;
  milestones_ = new QList<Milestone>;
}

#ifdef CRYPT
Project::Project(Crypt *crypt) {
  crypt_ = crypt;
  title_ = "";
  pro_dir_ = "";
  log_dir_ = "";
  log_file_ = "";
  stage_ = "";
  stage_comment_ = "";
  interval_ = 0;
  activities_ = new QList<Activity>;
  milestones_ = new QList<Milestone>;
}

Project::Project(Crypt *crypt, QString t) {
  crypt_ = crypt;
  title_ = t;
  pro_dir_ = "";
  log_dir_ = "";
  log_file_ = "";
  stage_ = "";
  stage_comment_ = "";
  interval_ = 0;
  activities_ = new QList<Activity>;
  milestones_ = new QList<Milestone>;
}
#endif

void Project::ClearProject() {
  title_ = "";
  pro_dir_ = "";
  log_dir_ = "";
  log_file_ = "";
  stage_ = "";
  stage_comment_ = "";
  interval_ = 0;
  activities_->clear();
  contributors_.clear();
  milestones_->clear();
}

void Project::SetTitle(QString t) {
  title_ = t;
}


QString Project::GetTitle() const {
  return title_;
}

void Project::SetProDir(QString d) {
  pro_dir_ = d;
}

QString Project::GetProDir() const {
  return pro_dir_;
}

void Project::SetLogDir(QString d) {
  log_dir_ = d;
}

QString Project::GetLogDir() const {
  return log_dir_;
}

void Project::SetLogFile(QString f) {
  log_file_ = f;
}

QString Project::GetLogFile() const {
  return log_file_;
}

void Project::AddActivity(Activity act) {
  act.SetId(activities_->length());
  activities_->append(act);
}

void Project::RemoveActivity(int n) {
  activities_->removeAt(n);
}

Activity Project::GetActivity (int n) const {
  return activities_->at(n);
}

void Project::SetContributor(QString c) {
  contributors_[0] = c;
}

void Project::SetContributor(QString c, int n) {
  contributors_[n] = c;
}

void Project::AddContributor(QString c) {
  if (!contributors_.contains(c))
    contributors_.append(c);
}

QString Project::GetContributor(int n) const {
  return contributors_.at(n);
}

QStringList Project::GetContributor() const {
  return contributors_;
}

int Project::GetInterval() const {
  return interval_;
}

void Project::SetInterval(const int &i) {
  interval_ = i;
}

void Project::SetInitStage(QString s) {
  stage_ = s;
}

QString Project::GetInitStage() const {
  return stage_;
}

void Project::SetStageComment(QString s) {
  stage_comment_ = s;
}

QString Project::GetStageComment() const {
  return stage_comment_;
}

void Project::AddMilestone(int e_id, QString title, QDateTime time,
                           QString comment, QString arc, QString threads_type,
                           QString threads, QString compiler, QString model,
                           QString perf_metric, QString perf_comment) {
  int m_id = milestones_->length();
  milestones_->append(Milestone());
  milestones_->last().SetMsId(m_id);
  milestones_->last().SetTitle(title);
  milestones_->last().SetTime(time);
  milestones_->last().SetArc(arc);
  milestones_->last().SetComment(comment);
  milestones_->last().SetModel(model);
  milestones_->last().SetPerfComment(perf_comment);
  milestones_->last().SetPerfMetric(perf_metric);
  milestones_->last().SetEventId(e_id);
  milestones_->last().SetThreadsType(threads_type);
  milestones_->last().SetThreadsComment(threads);
  milestones_->last().SetCompiler(compiler);
}

void Project::AddMilestone(Milestone *m) {
  int m_id = milestones_->length();
  m->SetMsId(m_id);
  milestones_->append(*m);
}

QList<Milestone> *Project::GetMilestone() {
  return milestones_;
}

Milestone Project::GetMilestone(int n) {
  return milestones_->at(n);
}

int Project::GetNoActivities() {
  return activities_->length();
}

bool Project::StoreLog(QString f) {
  QFile file(f);
  if (!file.open(QIODevice::WriteOnly)) {
    qWarning("Couldn't save file.");
    return false;
  }
  QJsonObject json;
  QJsonArray activities_array;
  foreach (const Activity a, *activities_) {
    QJsonObject obj;
    obj["ID"] = a.GetId();
    obj["Comment"] = a.GetComment();
    obj["NoEventsCurrentSession"] = a.GetSavedEvents();
    obj["LoggingInterval"] = a.GetLogInterval();
    obj["Interval"] = a.GetIntervalTime();
    obj["LastLoggingTime"] = a.GetLastTime().toString("yyyy-MM-dd hh:mm:ss");
    obj["CurLoggingTime"] = a.GetCurTime().toString("yyyy-MM-dd hh:mm:ss");
    obj["UserName"] = a.GetUserName();
    obj["ProjectTitle"] = a.GetProjectTitle();
    obj["ActivityType"] = a.GetType();
    activities_array.append(obj);
  }
  QJsonArray milestones_array;
  foreach (const Milestone ms, *milestones_) {
    QJsonObject obj;
    obj["Title"] = ms.GetTitle();
    obj["Time"] = ms.GetTime().toString("yyyy-MM-dd hh:mm:ss");
    obj["Comment"] = ms.GetComment();
    obj["ID"] = ms.GetMsId();
    obj["MatchingActivityID"] = ms.GetEventId();
    obj["PerfMetric"] = ms.GetPerfMetric();
    obj["PerfComment"] = ms.GetPerfComment();
    obj["ProgrammingModel"] = ms.GetModel();
    obj["ThreadsNodes"] = ms.GetThreadsType();
    obj["NoThreadsNodes"] = ms.GetThreadsComment();
    obj["Compiler"] = ms.GetCompiler();
    obj["Architecture"] = ms.GetArc();
    milestones_array.append(obj);
  }
  json["InitialProjectStage"] = stage_;
  json["InitialProjectStageComment"] = stage_comment_;
  json["Milestones"] = milestones_array;
  json["LoggingEvents"] = activities_array;

  QJsonDocument doc(json);
#ifdef CRYPT
  QByteArray data = doc.toJson();
  QByteArray enc_data;
  bool status = crypt_->Encrypt(data, enc_data);
  file.write(enc_data);
  return status;
#else
  file.write(doc.toJson());
  return true;
#endif
}

bool Project::ReadLog(QString f) {
  activities_->clear();
  QFile file(f);
  if (!file.open(QIODevice::ReadOnly)) {
    qWarning("Couldn't open log file.");
    return false;
  }
  QByteArray data = file.readAll();
#ifdef CRYPT
  QByteArray dec_data;
  bool status = crypt_->Decrypt(data, dec_data);
  QJsonDocument load_doc(QJsonDocument::fromJson(dec_data));
#else
  QJsonDocument load_doc(QJsonDocument::fromJson(data));
  bool status = true;
#endif
  QJsonObject json;
  json = load_doc.object();
  stage_ = json["InitialProjectStage"].toString();
  stage_comment_ = json["InitialProjectStageComment"].toString();
  QJsonArray milestones_array = json["Milestones"].toArray();
  foreach (const QJsonValue & v, milestones_array) {
    QJsonObject o = v.toObject();
    Milestone ms;
    ms.SetTitle(o["Title"].toString());
    ms.SetTime(QDateTime::fromString(o["Time"].toString(),"yyyy-MM-dd hh:mm:ss"));
    ms.SetComment(o["Comment"].toString());
    ms.SetMsId(o["ID"].toInt());
    ms.SetEventId(o["MatchingActivityID"].toInt());
    ms.SetPerfMetric(o["PerfMetric"].toString());
    ms.SetPerfComment(o["PerfComment"].toString());
    ms.SetModel(o["ProgrammingModel"].toString());
    ms.SetThreadsType(o["ThreadsNodes"].toString());
    ms.SetThreadsComment(o["NoThreadsNodes"].toString());
    ms.SetCompiler(o["Compiler"].toString());
    ms.SetArc(o["Architecture"].toString());
    milestones_->append(ms);
  }
  QJsonArray activities_array = json["LoggingEvents"].toArray();
  foreach (const QJsonValue & v, activities_array) {
    QJsonObject o = v.toObject();
    Activity a;
    a.SetId(o["ID"].toInt());
    a.SetType(o["ActivityType"].toString());
    if (VERBOSE)
      qDebug() << "ActivityType: " << o["ActivityType"].toString();
    a.SetProjectTitle(o["ProjectTitle"].toString());
    a.SetUserName(o["UserName"].toString());
    a.SetCurTime(QDateTime::fromString(o["CurLoggingTime"].toString(),"yyyy-MM-dd hh:mm:ss"));
    a.SetLastTime(QDateTime::fromString(o["LastLoggingTime"].toString(),"yyyy-MM-dd hh:mm:ss"));
    a.SetIntervalTime(o["Interval"].toInt());
    if (VERBOSE)
      qDebug() << "Interval: " << o["Interval"].toInt();
    a.SetLogInterval(o["LoggingInterval"].toInt());
    if (VERBOSE)
      qDebug() << a.GetId();
    a.SetSavedEvents(o["NoEventsCurrentSession"].toInt());
    a.SetComment(o["Comment"].toString());
    activities_->append(a);
  }
  return status;
}

bool Project::Load(QString f) {
  QFile file(f);
  if (!file.open(QIODevice::ReadOnly)) {
    qWarning("Couldn't open project file.");
    return false;
  }
  QByteArray data = file.readAll();
#ifdef CRYPT
  QByteArray dec_data;
  bool status = crypt_->Decrypt(data, dec_data);
  QJsonDocument load_doc(QJsonDocument::fromJson(dec_data));
#else
  QJsonDocument load_doc(QJsonDocument::fromJson(data));
  bool status = true;
#endif

  QJsonObject json;
  json = load_doc.object();
  title_ = json["ProjectTitle"].toString();
  pro_dir_ = json["ProjectDirectory"].toString();
  log_dir_ = json["LogFileDirectory"].toString();
  log_file_ = json["LogFile"].toString();
  interval_ = json["LoggingInterval"].toInt();
  QJsonArray array = json["Contributors"].toArray();
  foreach(const QJsonValue &cont, array) {
    AddContributor(cont.toObject().value("name").toString());
  }
  return status;
}

bool Project::Save(QString f) {
  QFile file(f);
  if (!file.open(QIODevice::ReadWrite)) {
    qWarning("Couldn't save file.");
    return false;
  }
  QJsonObject json;
  json["ProjectTitle"] = title_;
  json["ProjectDirectory"] = pro_dir_;
  json["LogFileDirectory"] = log_dir_;
  json["LogFile"] = log_file_;
  json["LoggingInterval"] = interval_;
  QJsonArray array;
  foreach (const QString s, contributors_) {
    QJsonObject obj;
    obj["name"] = s;
    array.append(obj);
  }
  json["Contributors"] = array;

  QJsonDocument doc(json);
#ifdef CRYPT
  QByteArray data = doc.toJson();
  QByteArray enc_data;
  bool status = crypt_->Encrypt(data, enc_data);
  file.write(enc_data);
  return status;
#else
  file.write(doc.toJson());
  return true;
#endif
}
