/* This file was created by Kureii (Tomas Adamek)
 * Date created: 30. 01. 2024
 * This file is under the GPL-3 license
 */

#pragma once

#include <string>
#include <vector>

namespace rain_text::register_login {

class Login {
  std::string username_;
  std::string password_;
  std::vector<uint8_t> key_;
public:
  Login(std::string username, std::string password);

};

}  // namespace rain_text::register_login