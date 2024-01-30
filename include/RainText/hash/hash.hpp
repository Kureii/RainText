/* This file was created by Kureii (Tomas Adamek)
 * Date created: 30. 01. 2024
 * This file is under the GPL-3 license
 */

#pragma once

#include <QString>
#include <vector>

namespace rain_text::hash {

class Hash {
 public:
  static QString GetDbName(QString username);
  static QString GetPswdHash(QString password, QString username);
  static std::vector<uint8_t> GetKey(QString password, QString username);
  static QString uint8_array_to_hex_string(const uint8_t* data, size_t length);
};

}  // namespace rain_text::hash
