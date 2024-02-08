/* This file was created by Kureii (Tomas Adamek)
 * Date created: 07. 02. 2024
 * This file is under the GPL-3 license
 */
//================================= Includes ===================================
#include "RainText/enrollment_manager.hpp"

#include <qtconcurrentrun.h>

#include <QFutureWatcher>
#include <utility>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

#include "RainText/encrypt_decrypt.hpp"
#include "RainText/user_db.hpp"

//================================= Namespace ==================================
namespace rain_text {
//================================= Public method ==============================
EnrollmentManager::EnrollmentManager(QObject *parent, QString path) : QObject(parent), path_(std::move(path)) {
  moveToThread(&workerThread_);
  connect(&workerThread_, &QThread::finished, this, &QObject::deleteLater);
  workerThread_.start();
}

EnrollmentManager::~EnrollmentManager() {
  stop();
  workerThread_.quit();
  workerThread_.wait();
}

void EnrollmentManager::start() {
  QMetaObject::invokeMethod(this, "processRecords", Qt::QueuedConnection);
}

void EnrollmentManager::stop() {
  QMutexLocker locker(&mutex_);
  stopRequested_ = true;
  condition_.wakeOne();
}

//================================= Public slots ===============================
void EnrollmentManager::processRecords(QList<RecordItem> listModel, int iterations) {
  QMutexLocker locker(&mutex_);
  QSqlDatabase db;
  if (!QSqlDatabase::contains("User")) {
    db = QSqlDatabase::addDatabase("QSQLITE", "User");
    db.setDatabaseName(path_);
  } else {
    db = QSqlDatabase::database("User");
  }

  QThreadPool pool;
  pool.setMaxThreadCount(MAX_THREADS - 2);

  while (!stopRequested_) {
    condition_.wait(&mutex_);
    std::vector<EncryptedRecordItem> eItems;
    size_t totalRecords = listModel.size();

    /*for (int i = 0; i < totalRecords; ++i) {
      auto eItem = EncryptDecrypt::EncryptRecordItem(listModel[i], key_, iterations);
      eItems.push_back(eItem);
    }*/

    auto key = key_;

    QVector<QFuture<EncryptedRecordItem>> futures;
    for (const auto& record : listModel) {
      QFuture<EncryptedRecordItem> future = QtConcurrent::run(&pool, [this, record, key, iterations]() {
          auto recordCopy = record;
          return EncryptDecrypt::EncryptRecordItem(recordCopy, key, iterations);
      });
      futures.append(future);
    }

    for (auto &future : futures) {
      future.waitForFinished();
      auto item = future.result();
      eItems.push_back(item);
    }


    QSqlQuery query(db);
    for (int i = 0; i < eItems.size(); ++i) {
      int id = i + 1;
      EncryptedRecordItem& item = eItems[i];

      query.prepare("INSERT OR REPLACE INTO data (id, headline, username, password, iterations) VALUES (?, ?, ?, ?, ?);");
      query.addBindValue(id);
      QByteArray headline(reinterpret_cast<const char*>(item.encryptedHeadline.data()), item.encryptedHeadline.size());
      query.addBindValue(headline);
      QByteArray username(reinterpret_cast<const char*>(item.encryptedUsername.data()), item.encryptedUsername.size());
      query.addBindValue(username);
      QByteArray password(reinterpret_cast<const char*>(item.encryptedPassword.data()), item.encryptedPassword.size());
      query.addBindValue(password);
      query.addBindValue(item.iterations);

      if (!query.exec()) {
        qWarning() << "Failed to insert or update item with id" << id << ":" << query.lastError().text();
      }
    }

    int maxId = eItems.size();
    query.prepare("DELETE FROM data WHERE id > ?;");
    query.addBindValue(maxId);

    if (!query.exec()) {
      qWarning() << "Failed to delete surplus items:" << query.lastError().text();
    }

    if (stopRequested_) break;
  }
  db.close();
  emit finished();
}
//================================= Testing method =============================
#ifdef ENABLE_TESTS

#endif

//================================= Private method =============================

//================================= End namespace ==============================
}  // namespace rain_text