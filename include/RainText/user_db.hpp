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

class UserDb {
  QString path_;
public:
  UserDb(const QString &path);

  std::vector<EncryptedRecordItem> GetAllRecords();

private:
  bool DbExist();
  QSqlDatabase GetDb();
  bool CreateTbale(QSqlDatabase &db);
};

}  // namespace rain_text
