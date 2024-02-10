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
#include "RainText/record_list_model.hpp"

namespace rain_text {

class EnrollmentManager : public QThread {
  Q_OBJECT

public:
  EnrollmentManager(QString &path, std::vector<uint8_t> &key, QThread *parent = nullptr);

  void run() override;

public slots:
  void addRecords(const QList<RecordItem> &listModel, int iterations);
  void terminate(const QList<RecordItem> &listModel, int iterations);

signals:
  void finished(); // Signál indikující ukončení zpracování
  void stopEnrollmentManager();

private:
  QMutex mutex_;
  QWaitCondition condition_;
  bool terminate_ = false;
  std::vector<uint8_t> key_;
  QString path_;
  QList<RecordItem> data_;
  int iterations_;
};

}  // namespace rain_text
