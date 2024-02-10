/* This file was created by Kureii (Tomas Adamek)
 * Date created: 30. 01. 2024
 * This file is under the GPL-3 license
 */
//================================= Includes ===================================
#include "RainText/register.hpp"

#include <QDir>
#include <QFile>
#include <QString>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

#include "RainText/hash.hpp"

//================================= Namespace ==================================
namespace rain_text::register_login {

//================================= Public method ==============================
Register::Register(QString username, QString password)
    : username_(username), password_(password) {}

bool Register::IsRegisterSusccessful(QString& path) {
  path = CreateUser(username_, password_);
  if (path.isEmpty()) {
    return false;
  } else {
    return true;
  }
}

std::vector<uint8_t> Register::GetKey() {
  return hash::Hash::GetKey(password_, username_);
}


//================================= Testing method =============================
#ifdef ENABLE_TESTS

#endif

//================================= Private method =============================
QString Register::CreateUser(const QString& username,
                                 const QString& password) {
  QDir dir(QFileInfo(MAIN_DB_PATH).absoluteDir());

  if (!dir.exists()) {
    dir.mkpath(".");
  }

  db_ = QSqlDatabase::addDatabase("QSQLITE");
  db_.setDatabaseName(MAIN_DB_PATH);

  if (!db_.open()) {
    qDebug() << "Cannot open database:" << db_.lastError().text();
    return {};
  }

  QString sql =
      "CREATE TABLE IF NOT EXISTS users("
      "hash BLOB UNIQUE NOT NULL,"
      "PRIMARY KEY (hash)"
      ");";

  QSqlQuery query;
  if (!query.exec(sql)) {
    qDebug() << "SQL execution error:" << query.lastError().text();
  }

  auto dbToken = rain_text::hash::Hash::GetPswdHash(password, username);
  QString insertSql = "INSERT INTO users (hash) VALUES (?);";
  QSqlQuery insertQuery;
  insertQuery.prepare(insertSql);
  insertQuery.addBindValue(dbToken);

  if (!insertQuery.exec()) {
    qDebug() << "SQL insert error:" << insertQuery.lastError().text();
    return {};
  }

  auto dbName = rain_text::hash::Hash::GetDbName(username);

  QString path = "./database/" + QString(dbName) + ".db";

  if (QFile::exists(path)) {
    return {};
  }

  db_.close();

  return path;

}

//================================= End namespace ==============================
}  // namespace rain_text::register_login