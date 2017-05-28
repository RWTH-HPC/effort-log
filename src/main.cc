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

#include <QApplication>
#include <QCoreApplication>
#include <QLibraryInfo>
#include <QLocale>
#include <QSettings>
#include <QTranslator>

#include <signal.h>

#include "definitions.h"
#include "mainwindow.h"
#include "milestone.h"
#include "milestonedialog.h"
#include "passworddialog.h"
#include "questionnairedialog.h"
#include "setupdialog.h"

#ifdef CRYPT
#include "crypt.h"
#endif

MainWindow *window;

void HandlerOnSignal(int param) {
  printf("\nQuit the application on signal %i.\n", param);
  window->QuitOnSignal();
  QCoreApplication::quit();
}

int main(int argc, char *argv[]) {
  QLocale::setDefault(QLocale::C);
  QApplication app(argc, argv);
  app.setOrganizationName("ITCenter");
  app.setOrganizationDomain("itc.rwth-aachen.de");
  app.setApplicationName("EffortLog");
  app.setApplicationVersion(APP_VERSION);

  QSettings settings;
  settings.setValue("noLoggedActivities", 0);
#ifdef CRYPT
  Crypt *crypt = new Crypt();
  window = new MainWindow(crypt);
  SetupDialog *setup = new SetupDialog(window, crypt);
#else
  window = new MainWindow();
  SetupDialog *setup = new SetupDialog(window);
#endif
  signal(SIGINT, HandlerOnSignal);
  setup->exec();
  if (settings.value("conf/confAccepted") == true) {
    return app.exec();
  } else {
    app.quit();
    return -1;
  }
}
