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
  stage_ = "";
  stage_comment_ = "";
  interval_ = -1;
  activities_ = new QList<Activity>;
}

Project::Project(QString t) {
  title_ = t;
  pro_dir_ = "";
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

void Project::SetLogFile(QString f) { log_file_ = f; }

QString Project::GetLogFile() const { return log_file_; }

void Project::AddActivity(Activity act) { activities_->append(act); }

void Project::RemoveActivity(int n) { activities_->removeAt(n); }

Activity Project::GetActivity(int n) const { return activities_->at(n); }

void Project::SetContributor(QString c) { contributors_[0] = c; }

void Project::SetContributor(QString c, int n) { contributors_[n] = c; }

void Project::AddContributor(QString c) {
  if (!contributors_.contains(c)) contributors_.append(c);
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
    if (a.GetMsId() != -1) ms.append(a);
  }
  return ms;
}

Activity Project::GetMilestone(int n) {
  foreach (const Activity a, *activities_) {
    if (a.GetMsId() == n) return a;
  }
  Activity empty_act;
  return empty_act;
}

int Project::GetNoMilestones() {
  int no_ms = 0;
  foreach (const Activity a, *activities_) {
    if (a.GetMsId() != -1) no_ms++;
  }
  return no_ms;
}

int Project::GetNoActivities() {
  if (activities_ != NULL)
    return activities_->length();
  else
    return 0;
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
    obj["ActivityType"] = a.GetType();
    obj["Architecture"] = a.GetArc();
    obj["Comment"] = a.GetComment();
    obj["Compiler"] = a.GetCompiler();
    obj["CurLoggingTime"] = a.GetCurTime().toString("yyyy-MM-dd hh:mm:ss");
    obj["DataSize"] = a.GetDataSize();
    obj["ID"] = a.GetId();
    obj["Interval"] = a.GetIntervalTime();
    obj["LastLoggingTime"] = a.GetLastTime().toString("yyyy-MM-dd hh:mm:ss");
    obj["LoggingInterval"] = a.GetLogInterval();
    obj["MsComment"] = a.GetMsComment();
    obj["MsID"] = a.GetMsId();
    obj["MsTitle"] = a.GetMsTitle();
    obj["NoEventsCurrentSession"] = a.GetSavedEvents();
    obj["NoThreadsNodes"] = a.GetThreadsComment();
    obj["PerfComment"] = a.GetPerfComment();
    obj["PerfMetric"] = a.GetPerfMetric();
    obj["ProgrammingModel"] = a.GetModel();
    obj["ProjectTitle"] = a.GetProjectTitle();
    obj["Scheduler"] = a.GetScheduler();
    obj["ThreadsNodes"] = a.GetThreadsType();
    obj["UserName"] = a.GetUserName();
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
  if (VERBOSE) qDebug() << "Log file: " << f;
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
    a.SetType(o["ActivityType"].toString());
    a.SetArc(o["Architecture"].toString());
    a.SetComment(o["Comment"].toString());
    a.SetCompiler(o["Compiler"].toString());
    a.SetCurTime(QDateTime::fromString(o["CurLoggingTime"].toString(),
                                       "yyyy-MM-dd hh:mm:ss"));
    a.SetDataSize(o["DataSize"].toString());
    a.SetId(o["ID"].toInt());
    a.SetIntervalTime(o["Interval"].toInt());
    a.SetLastTime(QDateTime::fromString(o["LastLoggingTime"].toString(),
                                        "yyyy-MM-dd hh:mm:ss"));
    a.SetLogInterval(o["LoggingInterval"].toInt());
    a.SetMsComment(o["MsComment"].toString());
    a.SetMsId(o["MsID"].toInt());
    a.SetMsTitle(o["MsTitle"].toString());
    a.SetSavedEvents(o["NoEventsCurrentSession"].toInt());
    a.SetThreadsComment(o["NoThreadsNodes"].toString());
    a.SetPerfComment(o["PerfComment"].toString());
    a.SetPerfMetric(o["PerfMetric"].toString());
    a.SetModel(o["ProgrammingModel"].toString());
    a.SetProjectTitle(o["ProjectTitle"].toString());
    if (o["Scheduler"].isUndefined())
      a.SetScheduler(0);
    else
      a.SetScheduler(o["Scheduler"].toInt());
    a.SetThreadsType(o["ThreadsNodes"].toString());
    a.SetUserName(o["UserName"].toString());
    activities_->append(a);
  }
  return status;
}

bool Project::Load(QString f) {
  QFile file(f);
  if (VERBOSE) qDebug() << "Project file: " << f;
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

QStringList Project::GetUniqueActivities() {
  QStringList list;
  foreach (const Activity a, *activities_) {
    if ((list.indexOf(a.GetType()) == -1) && (a.GetType() != ""))
      list.append(a.GetType());
  }
  return list;
}

QStringList Project::GetUniqueArchitectures() {
  QStringList list;
  foreach (const Activity a, *activities_) {
    if ((list.indexOf(a.GetArc()) == -1) && (a.GetArc() != ""))
      list.append(a.GetArc());
  }
  return list;
}

QStringList Project::GetUniqueCompilers() {
  QStringList list;
  foreach (const Activity a, *activities_) {
    if ((list.indexOf(a.GetCompiler()) == -1) && (a.GetCompiler() != ""))
      list.append(a.GetCompiler());
  }
  return list;
}

QStringList Project::GetUniqueProgModels() {
  QStringList list;
  foreach (const Activity a, *activities_) {
    if ((list.indexOf(a.GetModel()) == -1) && (a.GetModel() != ""))
      list.append(a.GetModel());
  }
  return list;
}

QStringList Project::GetUniqueDataSizes() {
  QStringList list;
  foreach (const Activity a, *activities_) {
    if ((list.indexOf(a.GetDataSize()) == -1) && (a.GetDataSize() != ""))
      list.append(a.GetDataSize());
  }
  return list;
}

QStringList Project::GetUniqueMs() {
  QStringList list;
  foreach (const Activity a, *activities_) {
    if ((list.indexOf(a.GetMsTitle()) == -1) && (a.GetMsTitle() != ""))
      list.append(a.GetMsTitle());
  }
  return list;
}

Activity Project::GetLastMs() {
  Activity act;
  if (GetNoMilestones() == 0) return act;
  int i = activities_->length() - 1;
  while (activities_->at(i).GetMsId() == -1 && i >= 0) i--;
  return activities_->at(i);
}

Activity Project::GetLastPerf() {
  Activity act;
  if (GetNoActivities() == 0) return act;
  int i = activities_->length() - 1;
  while (activities_->at(i).GetPerfComment() == "" && i >= 0) i--;
  return activities_->at(i);
}
