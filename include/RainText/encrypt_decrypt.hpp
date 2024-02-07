/* This file was created by Kureii (Tomas Adamek)
 * Date created: 07. 02. 2024
 * This file is under the GPL-3 license
 */

#pragma once
#include <vector>

#include "RainText/structs.hpp"

namespace rain_text {

class EncryptDecrypt {
 public:
  static RecordItem DecryptRecordItem(EncryptedRecordItem &eItem,
                                      const std::vector<uint8_t> &key);

  static EncryptedRecordItem EncryptRecordItem(RecordItem &item,
                                               const std::vector<uint8_t> &key,
                                               uint16_t iterations);

  static inline std::vector<uint8_t> QStringToUint8Vector(const QString &input);
};

}  // namespace rain_text
