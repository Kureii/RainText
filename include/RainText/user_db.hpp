/* This file was created by Kureii (Tomas Adamek)
 * Date created: 07. 02. 2024
 * This file is under the GPL-3 license
 */

#pragma once

#include <cstdint>
#include <vector>
#include <QtSql/QSqlDatabase>
#include "RainText/record_list_model.hpp"


namespace rain_text {

struct EncryptedRecordItem {
  std::vector<uint8_t> encryptedHeadline;
  std::vector<uint8_t> encryptedUsername;
  std::vector<uint8_t> encryptedPassword;
  int iterations;
};

class UserDb {
  std::vector<uint8_t> key_;
  QString path_;
public:
  UserDb(const std::vector<uint8_t> &key, const QString &path);

  std::vector<EncryptedRecordItem> GetAllRecords();

  model::RecordItem DecryptRecordItem(EncryptedRecordItem &eItem);

  EncryptedRecordItem EncryptRecordItem(model::RecordItem &item, uint16_t iterations);

private:
  bool DbExist();
  QSqlDatabase GetDb();
  bool CreateTbale(QSqlDatabase &db);
};

}  // namespace rain_text
