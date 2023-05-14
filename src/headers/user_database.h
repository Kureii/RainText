#ifndef RAINTEXT_USER_DATABASE_H
#define RAINTEXT_USER_DATABASE_H
//#define ENABLE_TESTS

#include <iostream>
#include <map>
#include <mutex>
#include <utility>
#include <vector>
#include <sqlite3.h>
#include <filesystem>

namespace rain_text {

struct Record{
  size_t content_id;
  std::string headline;
  std::string username;
  std::string password;

  bool operator==(const Record& other) const {
    return content_id == other.content_id &&
           headline == other.headline &&
           username == other.username &&
           password == other.password;
  }
};

struct EncryptRecord{
  size_t content_id;
  std::vector<uint8_t> data;
  std::vector<uint8_t> username;

  bool operator==(const EncryptRecord& other) const {
    return content_id == other.content_id &&
           data == other.data &&
           username == other.username;
  }
};

class UserDatabase{
  std::vector<uint8_t> id_manager_;
  std::map<std::string, std::vector<std::string>> map_data_;
  std::vector<EncryptRecord> encrypted_data_;
  std::vector<Record> plain_data_;
  std::vector<uint8_t> key_;
#ifdef ENABLE_TESTS
  std::filesystem::path db_path_;
#endif

 public:
  static bool gui_stopped_;
  static bool add_item_;
  explicit UserDatabase(const std::string& path);
  static sqlite3* connection_;
  size_t GetId();
  ~UserDatabase();
  std::vector<EncryptRecord> GetData();
  void SetData(const Record& record);
  void DeleteData(const int& id);
  void CreateIdManager();
  std::vector<Record> DecryptData(std::vector<uint8_t> key);
  [[nodiscard]] const std::vector<Record> &GetPlainData() const;
  void SetGuiStopped(bool guiStopped);
  void SetAddItem(bool addItem);
#ifdef ENABLE_TESTS
  void SetIdManager(std::vector<uint8_t> id_manager);
  std::vector<uint8_t> GetIdManager();
  void SetPlainData(std::vector<Record> plain_data);
  Record PrepareDataTest(char** text_data);
#endif
 private:
  void EnrolmentManager(UserDatabase * user_db);

};
}  // namespace rain_text

#endif  // RAINTEXT_USER_DATABASE_H
