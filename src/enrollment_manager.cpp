/* This file was created by Kureii (Tomas Adamek)
 * Date created: 07. 02. 2024
 * This file is under the GPL-3 license
 */
//================================= Includes ===================================
#include "RainText/enrollment_manager.hpp"

#include <qtconcurrentrun.h>

#include <QFutureWatcher>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <utility>

#include "RainText/encrypt_decrypt.hpp"
#include "RainText/user_db.hpp"

//================================= Namespace ==================================
namespace rain_text {
//================================= Public method ==============================
EnrollmentManager::EnrollmentManager(QString &path, std::vector<uint8_t> &key,
                                     QThread *parent)
    : QThread(parent), path_(path), key_(key) {
}

//================================= Protected method ===========================
void EnrollmentManager::run() {
  qDebug("run thread");
  QSqlDatabase db;
  if (!QSqlDatabase::contains("User")) {
    db = QSqlDatabase::addDatabase("QSQLITE", "User");
    db.setDatabaseName(path_);
  } else {
    db = QSqlDatabase::database("User");
  }

  if (!db.open()) {
    qDebug() << "Cannot open database:" << db.lastError().text();
  }

  QSqlQuery query(db);
  query.exec("PRAGMA journal_mode = WAL");

  QThreadPool pool;
  pool.setMaxThreadCount(MAX_THREADS - 2);
  while (!terminate_) {
    qDebug() << "waiting in thread";
    {
      QMutexLocker locker(&mutex_);
      condition_.wait(locker.mutex());
    }
    qDebug() << "mutex lock";
    if (!db.transaction()) {
      qWarning() << "Failed to start transaction:" << db.lastError();
      continue;
    }

    std::vector<EncryptedRecordItem> eItems;
    auto data = data_;
    auto iterations = iterations_;
    auto key = key_;
    qDebug() << "I get data!";
    if(data.empty()) {
      qDebug() << "data empty";
      if (terminate_) break;
    }

    QVector<QFuture<EncryptedRecordItem>> futures;
    for (const auto &record : data) {
      QFuture<EncryptedRecordItem> future =
          QtConcurrent::run(&pool, [this, record, key, iterations]() {
            auto recordCopy = record;
            return EncryptDecrypt::EncryptRecordItem(recordCopy, key,
                                                     iterations);
          });
      futures.append(future);
    }

    for (auto &future : futures) {
      future.waitForFinished();
      auto item = future.result();
      eItems.push_back(item);
    }

    for (int i = 0; i < eItems.size(); ++i) {
      int id = i + 1;
      EncryptedRecordItem &item = eItems[i];

      query.prepare(
          "INSERT OR REPLACE INTO data (id, headline, username, password, "
          "iterations) VALUES (?, ?, ?, ?, ?);");
      query.addBindValue(id);
      QByteArray headline(
          reinterpret_cast<const char *>(item.encryptedHeadline.data()),
          item.encryptedHeadline.size());
      query.addBindValue(headline);
      QByteArray username(
          reinterpret_cast<const char *>(item.encryptedUsername.data()),
          item.encryptedUsername.size());
      query.addBindValue(username);
      QByteArray password(
          reinterpret_cast<const char *>(item.encryptedPassword.data()),
          item.encryptedPassword.size());
      query.addBindValue(password);
      query.addBindValue(item.iterations);

      if (!query.exec()) {
        qWarning() << "Failed to insert or update item with id" << id << ":"
                   << query.lastError().text();
      }
    }

    int maxId = eItems.size();
    query.prepare("DELETE FROM data WHERE id > ?;");
    query.addBindValue(maxId);

    if (!query.exec()) {
      qWarning() << "Failed to delete surplus items:"
                 << query.lastError().text();
    }
    if (!db.commit()) {
      qWarning() << "Failed to commit transaction:" << db.lastError();
      db.rollback();
    }

    qDebug() << "Saving finished";
    if (terminate_) break;
  }
  qDebug() << "ending thread";
  db.close();
}

//================================= Public slots ===============================
void EnrollmentManager::addRecords(const QList<RecordItem> &listModel,
                                   int iterations) {
  qDebug() << "Add records from thread";
  {
    QMutexLocker locker(&mutex_);
    data_ = listModel;
    iterations_ = iterations;
    condition_.notify_one();
  }
  qDebug() << "Add records from end";
}

void EnrollmentManager::terminate(const QList<RecordItem> &listModel,
                                  int iterations) {
  {
    QMutexLocker locker(&mutex_);
    terminate_ = true;
    data_ = listModel;
    iterations_ = iterations;
    condition_.notify_one();
    wait();
  }
}

//================================= Testing method =============================
#ifdef ENABLE_TESTS

#endif

//================================= Private method =============================

//================================= End namespace ==============================
}  // namespace rain_text