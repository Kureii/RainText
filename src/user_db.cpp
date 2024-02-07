/* This file was created by Kureii (Tomas Adamek)
 * Date created: 07. 02. 2024
 * This file is under the GPL-3 license
 */
//================================= Includes ===================================
#include "RainText/user_db.hpp"

#include <QFile>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
//================================= Namespace ==================================
namespace rain_text {
//================================= Public method ==============================
UserDb::UserDb(const std::vector<uint8_t>& key, const QString& path)
    : key_(key), path_(path) {}

std::vector<EncryptedRecordItem> UserDb::GetAllRecords() {
  std::vector<EncryptedRecordItem> items;
  bool dbExist = DbExist();
  auto db = GetDb();
  if (!db.open()) {
    qDebug() << "Cannot open database:" << db.lastError().text();
    // emit error
    return items;
  }
  if (!dbExist) {
    if (!CreateTbale(db)) {
      // emit error
      return items;
    }
  }
  QSqlQuery query(db);
  if (!query.exec(
          "SELECT headline, username, password, iterations FROM data")) {
    qDebug() << "Load data error:" << query.lastError().text();
    // emit error
    return items;
  }

  while (query.next()) {
    EncryptedRecordItem item;
    QByteArray headlineBlob = query.value(0).toByteArray();
    item.encryptedHeadline =
        std::vector<uint8_t>(headlineBlob.begin(), headlineBlob.end());

    QByteArray usernameBlob = query.value(1).toByteArray();
    item.encryptedUsername =
        std::vector<uint8_t>(usernameBlob.begin(), usernameBlob.end());

    QByteArray passwordBlob = query.value(2).toByteArray();
    item.encryptedPassword =
        std::vector<uint8_t>(passwordBlob.begin(), passwordBlob.end());

    item.iterations = query.value(3).toInt();

    items.push_back(item);
  }

  return items;
}



//================================= Testing method =============================
#ifdef ENABLE_TESTS

#endif

//================================= Private method =============================

bool UserDb::DbExist() { return QFile::exists(path_); }

QSqlDatabase UserDb::GetDb() {
  QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
  db.setDatabaseName(path_);

  return db;
}

bool UserDb::CreateTbale(QSqlDatabase& db) {
  QSqlQuery query(db);
  bool success = query.exec(
      "CREATE TABLE IF NOT EXISTS data ("
      "id INTEGER PRIMARY KEY AUTOINCREMENT, "
      "headline BLOB, "
      "username BLOB, "
      "password BLOB,"
      "iterations INTEGER)");

  if (!success) {
    qDebug() << "Cannot create table:" << query.lastError().text();
    return false;
  }

  return true;
}

//================================= End namespace ==============================
}  // namespace rain_text
