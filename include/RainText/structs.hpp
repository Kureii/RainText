/* This file was created by Kureii (Tomas Adamek)
 * Date created: 07. 02. 2024
 * This file is under the GPL-3 license
 */

#pragma once

#include <QString>
#include <vector>

namespace rain_text {

struct EncryptedRecordItem {
  std::vector<uint8_t> encryptedHeadline;
  std::vector<uint8_t> encryptedUsername;
  std::vector<uint8_t> encryptedPassword;
  int iterations;
};

struct RecordItem {
  QString headlineText;
  QString usernameText;
  QString passwordText;
};

}  // namespace rain_text
