/*
 * Copyright (c) 2015-2022 by IT Center, RWTH Aachen University
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
#include <QJsonArray>
#include "definitions.h"

//! Class to handle projects.
/*!
 * This class holds all information to specify a project.
 * Additionally there is an interface to load from a file in JSON format or
 * save a list of activities to such file.
 */
class Project {
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
  //! Adds an appendix to the project
  /*!
   * \param[in] act The apendix the be added to the list of activities
   */
  void AddAppendix(Activity act);
  //! Returns an activity from the list of activities
  /*!
   * \param[in] n The number of activity to be returned
   * \return The activity with the given number n
   */
  Activity GetActivity(int n) const;
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
  //! Getter for all activities with milestones
  /*!
   * \return A pointer to a list of activities with milestones
   */
  QList<Activity> GetMilestone();
  //! Getter for milestone with ID n
  /*!
   * \param[in] n The ID of the milestone to look for.
   * \return A milestone from activities_
   */
  Activity GetMilestone(int n);
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
   * \param[in] f The name and directory of the JSON file
   * \return True if the file was successfully written, false if an error
   * occured
   */
  bool StoreLog(QString f);
  //! Reads the list of activities from disk
  /*!
   * Reads the list of activities from a JSON file and stores it in the list
   * of activities.
   * \param[in] f The name and directory of the JSON file
   * \return True if the file was successfully read, false if an error
   * occured
   */
  bool ReadLog(QString f);
  //! Stores the project to disk
  /*!
   * Stores the project to a project file
   * \param[in] f The name and directory of the project file
   * \return True if the file was successfully written, false if an error
   * occured
   */
  bool Save(QString f);
  //! Reads the list of activities from disk
  /*!
   * Reads the project from a project file and stores it project.
   * \param[in] f The name and directory of the project file
   * \return True if the file was successfully read, false if an error
   * occured
   */
  bool Load(QString f);
  //! Gets a list of  unique activities in the project
  /*!
   * Searches through all activities in the project to find unique activities
   * \return A list of strings of the unique activities in the project
   */
  QStringList GetUniqueActivities();
  //! Gets a list of  unique architectures in the project
  /*!
   * Searches through all activities in the project to find unique architectures
   * \return A list of strings of the unique architectures in the project
   */
  QStringList GetUniqueArchitectures();
  //! Gets a list of  unique compilers in the project
  /*!
   * Searches through all activities in the project to find unique compilers
   * \return A list of strings of the unique compilers in the project
   */
  QStringList GetUniqueCompilers();
  //! Gets a list of  unique programming models in the project
  /*!
   * Searches through all activities in the project to find unique programming
   * models \return A list of strings of the unique programming models in the
   * project
   */
  QStringList GetUniqueProgModels();
  //! Gets a list of  unique data sizes in the project
  /*!
   * Searches through all activities in the project to find unique data sizes
   * \return A list of strings of the unique data sizes in the project
   */
  QStringList GetUniqueDataSizes();
  //! Gets a list of  unique milestones in the project
  /*!
   * Searches through all activities in the project to find unique milestones
   * \return A list of strings of the unique milestones in the project
   */
  QStringList GetUniqueMs();
  //! Gets the last activity with a milestone
  /*!
   * Searches backwards through all activities in the project to find the last
   * milestone \return The last activity with a milestone
   */
  Activity GetLastMs();
  //! Gets the last activity with performance data
  /*!
   * Searches backwards through all activities in the project to find the last
   * performance measurement \return The last activity with performance data
   */
  Activity GetLastPerf();

 private:
#ifdef CRYPT
  Crypt *crypt_; /**< A pointer to Crypt which is associate with the current
                      project.*/
#endif
  QList<Activity> *activities_; /**< A list of activities associated with the
                                     current project.*/
  QString title_;               /**< The title of the project.*/
  QString pro_dir_;             /**< The working directory of the project.*/
  QString log_file_;            /**< The full path of the log file.*/
  QString stage_;               /**< The development stage of the project.*/
  QString stage_comment_;       /**< A comment about the development stage of
                                     the project.*/
  QStringList contributors_;    /**< A list of contributors to this project.*/
  int interval_;                /**< The logging interval.*/
};
