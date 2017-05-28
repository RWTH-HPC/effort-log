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

#include <QDateTime>
#include <QString>

//! Class to handle milestones
/*!
 * This class holds all information to specify milestones of a project.
 */
class Milestone {
public:
  //! The main constructor.
  Milestone();
  //! Clears all milestones
  void Clear();
  //! Setter for milestone_id_
  /*!
   * \param[in] id The id of the milestone
   */
  void SetMsId(int id);
  //! Getter for milestone_id_
  /*!
   * \return An integer containing milestone_id_
   */
  int GetMsId() const;
  //! Setter for event_id_
  /*!
   * \param[in] id The id of the corresponding event
   */
  void SetEventId(int id);
  //! Getter for event_id_
  /*!
   * \return An integer containing event_id_
   */
  int GetEventId() const;
  //! Setter for title_
  /*!
   * \param[in] t The title of the milestone
   */
  void SetTitle(QString t);
  //! Getter for title_
  /*!
   * \return A string containing title_
   */
  QString GetTitle() const;
  //! Setter for time_
  /*!
   * \param[in] t The time when the milestone was created
   */
  void SetTime(QDateTime t);
  //! Getter for time_
  /*!
   * \return A string containing time_
   */
  QDateTime GetTime() const;
  //! Setter for comment_
  /*!
   * \param[in] c A comment about the milestone
   */
  void SetComment(QString c);
  //! Getter for comment_
  /*!
   * \return A string containing comment_
   */
  QString GetComment() const;
  //! Setter for perf_metric_
  /*!
   * \param[in] m The metric for measuring the performance of the milestone
   */
  void SetPerfMetric(QString m);
  //! Getter for perf_metric_
  /*!
   * \return A string containing perf_metric_
   */
  QString GetPerfMetric() const;
  //! Setter for perf_comment_
  /*!
   * \param[in] c A comment about the performance of the milestone
   */
  void SetPerfComment(QString c);
  //! Getter for perf_comment_
  /*!
   * \return A string containing perf_comment_
   */
  QString GetPerfComment() const;
  //! Setter for arc_
  /*!
   * \param[in] a The architecture used during the milestone
   */
  void SetArc(QString a);
  //! Getter for arc_
  /*!
   * \return A string containing arc_
   */
  QString GetArc() const;
  //! Setter for threads_type_
  /*!
   * \param[in] t The metric used to specify nodes, threads, etc.
   */
  void SetThreadsType(QString t);
  //! Getter for threads_type_
  /*!
   * \return A string containing threads_type_
   */
  QString GetThreadsType() const;
  //! Setter for threads_comment_
  /*!
   * \param[in] c A comment on the metric used to specify nodes, threads, etc.
   */
  void SetThreadsComment(QString c);
  //! Getter for threads_comment_
  /*!
   * \return A string containing threads_comment_
   */
  QString GetThreadsComment() const;
  //! Setter for compiler_
  /*!
   * \param[in] c The compiler used during the milestone
   */
  void SetCompiler(QString c);
  //! Getter for compiler_
  /*!
   * \return A string containing compiler_
   */
  QString GetCompiler() const;
  //! Setter for model_
  /*!
   * \param[in] m The programming model used for the milestone
   */
  void SetModel(QString m);
  //! Getter for model_
  /*!
   * \return A string containing model_
   */
  QString GetModel() const;
  //! Setter for data_size_
  /*!
   * \param[in] d The data size used during the milestone
   */
  void SetDataSize(QString d);
  //! Getter for data_size_
  /*!
   * \return A string containing data_size_
   */
  QString GetDataSize() const;

private:
  int milestone_id_;        /**< The id of the milestone.*/
  int event_id_;            /**< The id of the corresponding event.*/
  QString title_;           /**< The title of the milestone.*/
  QDateTime time_;          /**< The time when the milestone was created.*/
  QString general_comment_; /**< A comment about the milestone.*/
  QString perf_metric_;     /**< The metric for measuring the performance of the
                                 milestone.*/
  QString perf_comment_;    /**< A comment about the performance of the
                                 milestone.*/
  QString arc_;             /**< The architecture used during the milestone.*/
  QString threads_type_; /**< The metric used to specify nodes, threads, etc.*/
  QString threads_comment_; /**< A comment on the metric used to specify
                                 nodes, threads, etc.*/
  QString compiler_;        /**< The compiler used during the milestone.*/
  QString model_;           /**< The programming model used for the milestone.*/
  QString data_size_;       /**< The data size used during the milestone.*/
};
