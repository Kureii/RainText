//
// Created by kureii on 7.2.24.
//

#pragma once
#include "RainText/structs.hpp"
#include <vector>

namespace rain_text {

class EncryptDecrypt {
public:
  static RecordItem DecryptRecordItem(EncryptedRecordItem &eItem, std::vector<uint8_t> &key);

  static EncryptedRecordItem EncryptRecordItem(RecordItem &item, std::vector<uint8_t> &key, uint16_t iterations);

  static inline std::vector<uint8_t> QStringToUint8Vector(const QString& input);
};

} // rain_text
