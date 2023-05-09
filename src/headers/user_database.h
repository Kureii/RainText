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

class UserDatabase {
  std::vector<uint8_t> id_manager_;
  std::mutex db_mutex_;
  std::vector<uint8_t> encoded_data_;
  std::map<std::string, std::vector<std::string>> map_data_;
  std::vector<Record> plain_data_;
  void CreateIdManager();
  void CreateMap();
#ifdef ENABLE_TESTS
  std::filesystem::path db_path_;
#endif

 public:
  explicit UserDatabase(const std::string& path);
  static sqlite3* connection_;
  size_t GetId();
  ~UserDatabase();
  std::vector<Record> GetData();
  void SetData(const Record& record);
  void DeleteData(const int& id);
#ifdef ENABLE_TESTS
  void SetIdManager(std::vector<uint8_t> id_manager);
  std::vector<uint8_t> GetIdManager();
  void SetPlainData(std::vector<Record> plain_data);
  std::vector<Record> GetPlainData();
  Record PrepareDataTest(char** text_data);
#endif

};
}  // namespace rain_text

#endif  // RAINTEXT_USER_DATABASE_H
