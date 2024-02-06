/* This file was created by Kureii (Tomas Adamek)
 * Date created: 30. 01. 2024
 * This file is under the GPL-3 license
 */

#pragma once

#include <QString>
#include <vector>
#include <QtSql/QSqlDatabase>

namespace rain_text::register_login {

class Register {
  QString username_;
  QString password_;
  QSqlDatabase db_;
  std::vector<uint8_t> key_;
public:
  Register(QString username, QString password);

  bool IsRegisterSusccessful(QString &path);

private:
  QString CreateUser(const QString& username, const QString& password);
};

}  // namespace rain_text::register_login