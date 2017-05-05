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

#pragma once

#include "activity.h"
#ifdef CRYPT
#include "crypt.h"
#endif
#include "definitions.h"
#include "milestone.h"
#include <QJsonArray>


//! Class to handle projects.
/*!
 * This class holds all information to specify a project.
 * Additionally there is an interface to load from a file in JSON format or
 * save a list of activities to such file.
 */
class Project
{
public:
  //! The main constructor.
  Project();
  //! Construction which also sets title_
  /*!
   * \param[in] t The title of the project
   */
  Project(QString t);
  //! The main constructor.
  /*!
   * \param[in] crypt Handles encryption and decryption for the project
   */
#ifdef CRYPT
  Project(Crypt *crypt);
#endif
  //! Construction which also sets title_
  /*!
   * \param[in] crypt Handles encryption and decryption for the project
   * \param[in] t The title of the project
   */
#ifdef CRYPT
  Project(Crypt *crypt, QString t);
#endif
  //! Clears the whole project
  void ClearProject();
  //! Setter for title_
  /*!
   * \param[in] t The title of the project
   */
  void SetTitle(QString t);
  //! Getter for title_
  /*!
   * \return A string containing title_
   */
  QString GetTitle() const;
  //! Setter for pro_dir_
  /*!
   * \param[in] d The working directory of the project
   */
  void SetProDir(QString d);
  //! Getter for pro_dir_
  /*!
   * \return A string containing pro_dir_
   */
  QString GetProDir() const;
  //! Setter for log_dir_
  /*!
   * \param[in] d The directory to save the log files to
   */
  void SetLogDir(QString d);
  //! Getter for log_dir_
  /*!
   * \return A string containing log_dir_
   */
  QString GetLogDir() const;
  //! Setter for log_file_
  /*!
   * \param[in] f The full path of the log file
   */
  void SetLogFile(QString f);
  //! Getter for log_file_
  /*!
   * \return A string containing log_file_
   */
  QString GetLogFile() const;
  //! Adds an activity to the project
  /*!
  * \param[in] act The Activity the be added to the list of activities
  */
  void AddActivity(Activity act);
  //! Removes an activity from the project
  /*!
  * \param[in] n The number of activity to be removed
  */
  void RemoveActivity(int n);
  //! Returns an activity from the list of activities
  /*!
  * \param[in] n The number of activity to be returned
  * \return The activity with the given number n
  */
  Activity GetActivity (int n) const;
  //! Setter for the first contributor of contributors_
  /*!
   * \param[in] c The name of a contributor
   */
  void SetContributor(QString c);
  //! Adds a contributor at position n of contributors_
  /*!
   * \param[in] c The name of a contributor
   * \param[in] n The position to set the contributor to
   */
  void SetContributor(QString c, int n);
  //! Adds a contributor to the end of contributors_
  /*!
   * \param[in] c The name of a contributor
   */
  void AddContributor(QString c);
  //! Getter for contributors_ at position n
  /*!
   * \return A string containing contributor_[n]
   */
  QString GetContributor(int n) const;
  //! Getter for all contributors
  /*!
   * \return A string containing contributors_
   */
  QStringList GetContributor() const;
  //! Getter for interval_
  /*!
   * \return An interger value containing interval_
   */
  int GetInterval() const;
  //! Setter for interval_
  /*!
   * \param[in] i An interger value which specifies the logging
   * interval in minutes which should be set
   */
  void SetInterval(const int &i);
  //! Setter for stage_
  /*!
   * \param[in] s The development stage of the project.
   */
  void SetInitStage(QString s);
  //! Getter for stage_
  /*!
   * \return A string containing stage_
   */
  QString GetInitStage() const;
  //! Setter for stage_comment_
  /*!
   * \param[in] s A comment about the development stage of the project.
   */
  void SetStageComment(QString s);
  //! Getter for stage_comment_
  /*!
   * \return A string containing stage_comment_
   */
  QString GetStageComment() const;
  //! Add a milestone to the project
  /*!
   * \param[in] e_id The id of the associate logging event.
   * \param[in] title The title of the milestone.
   * \param[in] time The time of the milestone.
   * \param[in] comment A comment about the milestone.
   * \param[in] arc The architecture used during the milestone.
   * \param[in] threads_type The metric used to specify nodes, threads, etc.
   * \param[in] threads The number of nodes, threads, etc.
   * \param[in] compiler The compiler used during the milestone.
   * \param[in] model The programming model used for the milestone.
   * \param[in] perf_metric The metric for measuring the performance of the
   * milestone
   * \param[in] perf_comment A comment about the performance of the milestone.
   * \param[in] data_size A comment about the data size of the milestone.
   */
  void AddMilestone(int e_id, QString title, QDateTime time, QString comment,
                    QString arc, QString threads_type, QString threads,
                    QString compiler, QString model, QString perf_metric,
                    QString perf_comment, QString data_size);
  //! Add a milestone to the project
  /*!
   * \param[in] m The milestone to be added to the porject.
   */
  void AddMilestone(Milestone *m);
  //! Getter for milestones_
  /*!
   * \return A pointer to milestones_
   */
  QList<Milestone> *GetMilestone();
  //! Getter for milestones_ at position n
  /*!
   * \param[in] n The position of the milestone to look for.
   * \return A milestones from milestones_
   */
  Milestone GetMilestone(int n);
  //! Gets the number of milestones in the current project
  /*!
   * \return The number of milestones in the current project.
   */
  int GetNoMilestones();
  //! Gets the number of activities in the current project
  /*!
   * \return The number of activities in the current project.
   */
  int GetNoActivities();
  //! Stores the list of activities to disk
  /*!
   * Stores the list of activities to a JSON file
   * \param[in] f The name and direktory of the JSON file
   * \return True if the file was successfully written, false if an error
   * occured
   */
  bool StoreLog(QString f);
  //! Reads the list of activities from disk
  /*!
  * Reads the list of activities from a JSON file and stores it in the list
  * of activities.
  * \param[in] f The name and direktory of the JSON file
  * \return True if the file was successfully read, false if an error
  * occured
  */
  bool ReadLog(QString f);
  //! Stores the project to disk
  /*!
   * Stores the project to a project file
   * \param[in] f The name and direktory of the project file
   * \return True if the file was successfully written, false if an error
   * occured
   */
  bool Save(QString f);
  //! Reads the list of activities from disk
  /*!
  * Reads the project from a project file and stores it project.
  * \param[in] f The name and direktory of the project file
  * \return True if the file was successfully read, false if an error
  * occured
  */
  bool Load(QString f);

private:
#ifdef CRYPT
  Crypt *crypt_;  /**< A pointer to Crypt which is associate with the current
                       project.*/
#endif
  QList<Activity> *activities_;  /**< A list of activities associated with the
                                      current project.*/
  QString title_;  /**< The title of the project.*/
  QString pro_dir_;  /**< The working directory of the project.*/
  QString log_dir_;  /**< The directory to store log files to.*/
  QString log_file_;  /**< The full path of the log file.*/
  QString stage_;  /**< The development stage of the project.*/
  QString stage_comment_;  /**< A comment about the development stage of the
                                project.*/
  QStringList contributors_;  /**< A list of contributors to this project.*/
  int interval_;  /**< The logging interval.*/
  QList<Milestone> *milestones_;  /**< A list of milestones linked to this
                                       project.*/
};
