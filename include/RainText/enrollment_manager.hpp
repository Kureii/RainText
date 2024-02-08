/* This file was created by Kureii (Tomas Adamek)
 * Date created: 07. 02. 2024
 * This file is under the GPL-3 license
 */

#pragma once

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <vector>
#include "RainText/structs.hpp"

namespace rain_text {

class EnrollmentManager : public QObject {
  Q_OBJECT

public:
  EnrollmentManager(QObject *parent = nullptr, QString path = "");
  ~EnrollmentManager();

  void start(); // Funkce pro spuštění zpracování v samostatném vlákně
  void stop();  // Funkce pro bezpečné ukončení vlákna

  public slots:
      void processRecords(QList<RecordItem> listModel, int iterations); // Slot pro zpracování záznamů

  signals:
      void finished(); // Signál indikující ukončení zpracování

private:
  QThread workerThread_;
  QMutex mutex_;
  QWaitCondition condition_;
  bool stopRequested_ = false;
  std::vector<uint8_t> key_;
  QString path_;
};

}  // namespace rain_text
