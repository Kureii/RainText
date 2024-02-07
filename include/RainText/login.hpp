/* This file was created by Kureii (Tomas Adamek)
 * Date created: 30. 01. 2024
 * This file is under the GPL-3 license
 */

#pragma once

#include <string>
#include <vector>
#include <QString>

namespace rain_text::register_login {

class Login {
  QString username_;
  QString password_;
  std::vector<uint8_t> key_;
public:
  Login(QString username, QString password);

  bool IsLoginSusccessful(QString &path);
  std::vector<uint8_t> GetKey();

private:
  bool VerifyUser(QString &path);
};


}  // namespace rain_text::register_login