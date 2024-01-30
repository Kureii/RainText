/* This file was created by Kureii (Tomas Adamek)
 * Date created: 30. 01. 2024
 * This file is under the GPL-3 license
 */
//================================= Includes ===================================
#include "RainText/register_login/register.hpp"

#include <QString>
#include <QFile>
#include <QDir>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>


//================================= Namespace ==================================
namespace rain_text::register_login {

//================================= Public method ==============================
Register::Register(QString username, QString password)
    : username_(username), password_(password) {}
bool Register::IsRegisterSusccessful(QString& path) {}

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
    return QString();
  }

  QString sql =
      "CREATE TABLE IF NOT EXISTS users("
      "hash TEXT UNIQUE NOT NULL,"
      "PRIMARY KEY (hash)"
      ");";

  QSqlQuery query;
  if (!query.exec(sql)) {
    qDebug() << "SQL execution error:" << query.lastError().text();
  }

  auto dbName = "";//hash::GetDbName(username);

  QString path = "./database/" + QString(dbName) + ".db";

  if (QFile::exists(path)) {
    return QString();
  }

  db_.close();

  return path;

}

//================================= End namespace ==============================
}  // namespace rain_text::register_login