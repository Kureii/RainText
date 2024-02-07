/* This file was created by Kureii (Tomas Adamek)
 * Date created: 30. 01. 2024
 * This file is under the GPL-3 license
 */
//================================= Includes ===================================
#include <utility>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

#include "RainText/login.hpp"
#include "RainText/hash.hpp"

//================================= Namespace ==================================
namespace rain_text::register_login {
//================================= Public method ==============================
 Login::Login(QString username, QString password) : password_(std::move(password)), username_(std::move(username)) {}

bool Login::IsLoginSusccessful(QString &path) {
   return VerifyUser(path);
 }

std::vector<uint8_t> Login::GetKey() {
   return hash::Hash::GetKey(password_,username_);
 }


//================================= Testing method =============================
#ifdef ENABLE_TESTS

#endif

//================================= Private method =============================
bool Login::VerifyUser(QString& path) {
   path = "";
   QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
   db.setDatabaseName(MAIN_DB_PATH);

   if (!db.open()) {
     qDebug() << "Cannot open database:" << db.lastError().text();
     return false;
   }

   QByteArray passwordHash = hash::Hash::GetPswdHash(password_, username_);

   QString sql = "SELECT hash FROM users WHERE hash = :hash;";
   QSqlQuery query;
   query.prepare(sql);
   query.bindValue(":hash", passwordHash);

   if (!query.exec()) {
     qDebug() << "SQL execution error:" << query.lastError().text();
     db.close();
     return false;
   }

   if (query.next()) {
     path = "./database/" + hash::Hash::GetDbName(username_) + ".db";
   } else {
     qDebug() << "User not found or wrong password";
     return false;
   }

   db.close();

   return true;
 }


//================================= End namespace ==============================
}  // namespace rain_text::register_login