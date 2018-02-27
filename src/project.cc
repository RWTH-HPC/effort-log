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

#include "project.h"
#include <QMessageBox>

#include <QDebug>

Project::Project() {
  title_ = "";
  pro_dir_ = "";
  log_dir_ = "";
  log_file_ = "";
  stage_ = "";
  stage_comment_ = "";
  interval_ = -1;
  activities_ = new QList<Activity>;
}

Project::Project(QString t) {
  title_ = t;
  pro_dir_ = "";
  log_dir_ = "";
  log_file_ = "";
  stage_ = "";
  stage_comment_ = "";
  interval_ = -1;
  activities_ = new QList<Activity>;
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
}

void Project::SetTitle(QString t) { title_ = t; }

QString Project::GetTitle() const { return title_; }

void Project::SetProDir(QString d) { pro_dir_ = d; }

QString Project::GetProDir() const { return pro_dir_; }

void Project::SetLogDir(QString d) { log_dir_ = d; }

QString Project::GetLogDir() const { return log_dir_; }

void Project::SetLogFile(QString f) { log_file_ = f; }

QString Project::GetLogFile() const { return log_file_; }

void Project::AddActivity(Activity act) { activities_->append(act); }

void Project::RemoveActivity(int n) { activities_->removeAt(n); }

Activity Project::GetActivity(int n) const { return activities_->at(n); }

void Project::SetContributor(QString c) { contributors_[0] = c; }

void Project::SetContributor(QString c, int n) { contributors_[n] = c; }

void Project::AddContributor(QString c) {
  if (!contributors_.contains(c))
    contributors_.append(c);
}

QString Project::GetContributor(int n) const { return contributors_.at(n); }

QStringList Project::GetContributor() const { return contributors_; }

int Project::GetInterval() const { return interval_; }

void Project::SetInterval(const int &i) { interval_ = i; }

void Project::SetInitStage(QString s) { stage_ = s; }

QString Project::GetInitStage() const { return stage_; }

void Project::SetStageComment(QString s) { stage_comment_ = s; }

QString Project::GetStageComment() const { return stage_comment_; }

QList<Activity> Project::GetMilestone() {
  QList<Activity> ms;
  foreach (const Activity a, *activities_) {
    if (a.GetMsId() != -1)
      ms.append(a);
  }
  return ms;
}

Activity Project::GetMilestone(int n) {
  foreach (const Activity a, *activities_) {
    if (a.GetMsId() == n)
      return a;
  }
  Activity empty_act;
  return empty_act;
}

int Project::GetNoMilestones() {
  int no_ms = 0;
  foreach (const Activity a, *activities_) {
    if (a.GetMsId() != -1)
      no_ms++;
  }
  return no_ms;
}

int Project::GetNoActivities() { return activities_->length(); }

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
    obj["Comment"] = a.GetMsComment();
    obj["NoEventsCurrentSession"] = a.GetSavedEvents();
    obj["LoggingInterval"] = a.GetLogInterval();
    obj["Interval"] = a.GetIntervalTime();
    obj["LastLoggingTime"] = a.GetLastTime().toString("yyyy-MM-dd hh:mm:ss");
    obj["CurLoggingTime"] = a.GetCurTime().toString("yyyy-MM-dd hh:mm:ss");
    obj["UserName"] = a.GetUserName();
    obj["ProjectTitle"] = a.GetProjectTitle();
    obj["ActivityType"] = a.GetType();
    obj["Scheduler"] = a.GetScheduler();
    obj["MsTitle"] = a.GetMsTitle();
    obj["MsComment"] = a.GetMsComment();
    obj["MsID"] = a.GetMsId();
    obj["PerfMetric"] = a.GetPerfMetric();
    obj["PerfComment"] = a.GetPerfComment();
    obj["ProgrammingModel"] = a.GetModel();
    obj["ThreadsNodes"] = a.GetThreadsType();
    obj["NoThreadsNodes"] = a.GetThreadsComment();
    obj["Compiler"] = a.GetCompiler();
    obj["Architecture"] = a.GetArc();
    obj["DataSize"] = a.GetDataSize();
    activities_array.append(obj);
  }
  json["InitialProjectStage"] = stage_;
  json["InitialProjectStageComment"] = stage_comment_;
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
  if (VERBOSE)
    qDebug() << "Log file: " << f;
  QFile file(f);
  if (!file.open(QFile::ReadOnly)) {
    QMessageBox::information(0, "Error", file.errorString());
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
  QJsonArray activities_array = json["LoggingEvents"].toArray();
  foreach (const QJsonValue &v, activities_array) {
    QJsonObject o = v.toObject();
    Activity a;
    a.SetId(o["ID"].toInt());
    a.SetType(o["ActivityType"].toString());
    if (VERBOSE)
      qDebug() << "ActivityType: " << o["ActivityType"].toString();
    a.SetProjectTitle(o["ProjectTitle"].toString());
    a.SetUserName(o["UserName"].toString());
    a.SetCurTime(QDateTime::fromString(o["CurLoggingTime"].toString(),
                                       "yyyy-MM-dd hh:mm:ss"));
    a.SetLastTime(QDateTime::fromString(o["LastLoggingTime"].toString(),
                                        "yyyy-MM-dd hh:mm:ss"));
    a.SetIntervalTime(o["Interval"].toInt());
    if (VERBOSE)
      qDebug() << "Interval: " << o["Interval"].toInt();
    a.SetLogInterval(o["LoggingInterval"].toInt());
    if (VERBOSE)
      qDebug() << a.GetId();
    a.SetSavedEvents(o["NoEventsCurrentSession"].toInt());
    a.SetMsComment(o["Comment"].toString());
    if (o["Scheduler"].isUndefined())
      a.SetScheduler(0);
    else
      a.SetScheduler(o["Scheduler"].toInt());
    a.SetMsTitle(o["MsTitle"].toString());
    a.SetMsComment(o["MsComment"].toString());
    a.SetMsId(o["MsID"].toInt());
    a.SetId(o["MatchingActivityID"].toInt());
    a.SetPerfMetric(o["PerfMetric"].toString());
    a.SetPerfComment(o["PerfComment"].toString());
    a.SetModel(o["ProgrammingModel"].toString());
    a.SetThreadsType(o["ThreadsNodes"].toString());
    a.SetThreadsComment(o["NoThreadsNodes"].toString());
    a.SetCompiler(o["Compiler"].toString());
    a.SetArc(o["Architecture"].toString());
    a.SetArc(o["DataSize"].toString());
    activities_->append(a);
  }
  return status;
}

bool Project::Load(QString f) {
  QFile file(f);
  if (VERBOSE)
    qDebug() << "Project file: " << f;
  if (!file.open(QFile::ReadOnly)) {
    throw QFile::OpenError;
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
  stage_ = json["StageOfDevel"].toInt();
  stage_comment_ = json["Comment"].toInt();
  QJsonArray array = json["Contributors"].toArray();
  foreach (const QJsonValue &cont, array) {
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
  json["StageOfDevel"] = stage_;
  json["Comment"] = stage_comment_;
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
