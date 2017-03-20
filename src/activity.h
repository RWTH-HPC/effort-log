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

#ifndef ACTIVITY_H
#define ACTIVITY_H

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSettings>
#include <QString>
#include <QTime>

#include "definitions.h"

//! Class to handle different activities.
/*!
 * This class holds all information to specify an activity.
 * Additionally there is an interface to load from a file in JSON format or
 * save an activity to such file.
 */
class Activity {
public:
  //! An array of strings containing the different types of the activities.
  /*!
   * Can be accessed with kActivityType[0] to kActivityType[NUM_ACTIVITIES - 1]
   * \see DEFINITIONS
   */
  static const QString kActivityType[];
  //! The main constructor.
  Activity();
  //! Getter for kActivityType
  /*!
   * \return A string containing the kActivityType
   */
  QString GetType() const;
  //! Setter for kActivityType
  /*!
   * \param[in] type A string which specifies the activity which should be set
   */
  void SetType(QString type);
  //! Getter for id_
  /*!
   * \return An interger value containing id_
   */
  int GetId() const;
  //! Setter for id_
  /*!
   * \param[in] i An interger which specifies the id of the activity
   */
  void SetId(int i);
  //! Getter for saved_events_
  /*!
   * \return An interger value containing saved_events_
   */
  int GetSavedEvents() const;
  //! Setter for saved_events_
  /*!
   * \param[in] events An integer value which denotes the number of saved
   * events to be set
   */
  void SetSavedEvents(const int &events);
  //! Getter for project_title_
  /*!
   * \return A string containing _project_title
   */
  QString GetProjectTitle() const;
  //! Setter for project_title_
  /*!
   * \param[in] title A string which specifies the project's title to be set
   */
  void SetProjectTitle(const QString &title);
  //! Getter for user_name_
  /*!
   * \return A string containing user_name_
   */
  QString GetUserName() const;
  //! Setter for user_name_
  /*!
   * \param[in] name A string which specifies the user's name to be set
   */
  void SetUserName(const QString &name);
  //! Getter for log_interval_
  /*!
   * \return An interger value containing log_interval_
   */
  int GetLogInterval() const;
  //! Setter for log_interval_
  /*!
   * \param[in] interval An interger value which specifies the logging
   * interval in minutes which should be set
   */
  void SetLogInterval(const int &interval);
  //! Getter for cur_time_
  /*!
   * \return A variable of type QTime containing cur_time_
   */
  QDateTime GetCurTime() const;
  //! Setter for cur_time_
  /*!
   * \param[in] time A variable of type QTime which specifies the current time
   */
  void SetCurTime(const QDateTime &time);
  //! Getter for last_time_
  /*!
   * \return A variable of type QTime containing last_time_
   */
  QDateTime GetLastTime() const;
  //! Setter for last_time_
  /*!
   * \param[in] time A variable of type QTime which specifies the time of the
   * lastly done log
   */
  void SetLastTime(const QDateTime &time);
  //! Getter for interval_time_
  /*!
   * \return An interger value containing interval_time_
   */
  int GetIntervalTime() const;
  //! Setter for interval_time_
  /*!
   * \param[in] time An interger value which specifies the time between
   * the current activity and the last one
   */
  void SetIntervalTime(const int &time);
  //! Getter for comment_
  /*!
   * \return A string containing comment_
   */
  QString GetComment() const;
  //! Setter for comment_
  /*!
   * \param[in] comment A string which specifies the comment specified by the
   * user
   */
  void SetComment(const QString &comment);
  //! Getter for scheduler_
  /*!
   * \return An integer containing scheduler_
   */
  int GetScheduler() const;
  //! Setter for scheduler_
  /*!
   * \param[in] scheduler An integer which specifies how the event was created
   */
  void SetScheduler(const int scheduler);

private:
  QString type_;          /**< A string which holds the type of activity.*/
  int id_;                /**< Holds the ID of Activity::type_*/
  int saved_events_;      /**< An integer value which holds a count of saved
                                events.*/
  QString project_title_; /**< A string which holds the project's title.*/
  QString user_name_;     /**< A string which holds the user's name.*/
  int log_interval_;   /**< An integer value which holds the logging interval in
                             minutes.*/
  QDateTime cur_time_; /**< A variable of type QDateTime which holds current
                            date and time.*/
  QDateTime last_time_; /**< A variable of type QDateTime which holds the time
                             when the last logging event happend.*/
  int interval_time_;   /**< An integer value which holds the actual logging
                             interval in minutes (cur_time_ - last_time_).*/
  QString comment_; /**< A string which holds an user-specified comment on his
                         or her activty.*/
  int scheduler_; /**< Stores how the event was created. 0 denotes an undefined
                   value, 1 denotes an
                   interval-based event , 2 an appeneded event, 3 a manual
                   event executed through the GUI, and 4 an event on closing the
                   program.*/
};

#endif // ACTIVITY
