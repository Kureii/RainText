//================================= Includes ===================================
#include "headers/user_database.h"
#include "headers/database_utils.h"

#include <algorithm>
#include <random>
#include <string>
#include <thread>

//================================= Namespace ==================================
namespace rain_text {
//======================== Define helpful variables ============================
uint8_t separators[] = {0x1C, 0x1D, 0x1E, 0x1F};

//======================= Define helpful structures ============================

//======================= Define helpful functions =============================
static int PrepareData(void *user_data, int argc, char **argv,
                       char **azColName);

void PrintVector(const char *place, const std::vector<uint8_t> &vec);
void PrintMap(const char *place, std::map<std::string, std::vector<std::string>> map_data);

//====================== UserDatabase implementation ===========================
sqlite3 *UserDatabase::connection_ = nullptr;

UserDatabase::UserDatabase(const std::string &path) {
  std::filesystem::path db_path(path);
  std::filesystem::path db_dir = db_path.parent_path();

  database_utils::CreateDir(db_dir);

  database_utils::OpenDb(path.c_str(), connection_);

  auto sql = std::string(
      "CREATE TABLE IF NOT EXISTS username("
      "username_id TEXT UNIQUE NOT NULL,"
      "PRIMARY KEY (username_id)"
      ");"
      "CREATE TABLE IF NOT EXISTS content ("
      "content_id INTEGER PRIMARY KEY AUTOINCREMENT,"
      "username_id TEXT NOT NULL,"
      "data TEXT,"
      "FOREIGN KEY (username_id) REFERENCES Username (username_id) ON DELETE "
      "CASCADE"
      ");");

  database_utils::ExecuteSql(connection_ ,sql);
  /*if (sqlite3_exec(connection_, sql, nullptr, nullptr, &err_msg) != SQLITE_OK) {
    fprintf(stderr, "SQL error: %s\n", err_msg);
    sqlite3_free(err_msg);
    sqlite3_close(connection_);
  }*/
#ifdef ENABLE_TESTS
  db_path_ = db_dir;
#endif

}

UserDatabase::~UserDatabase() {
  sqlite3_close(connection_);
#ifdef ENABLE_TESTS
  try {
    std::filesystem::remove_all(db_path_);
  } catch (const std::filesystem::filesystem_error& e) {
    std::cerr << "Error deleting path and contents: " << e.what() << std::endl;
  }
#endif
}

std::vector<Record> UserDatabase::GetData() {
  const char *sql = "SELECT username_id, content_id, data FROM content;";

  database_utils::ExecuteSql(connection_ ,sql, PrepareData, &plain_data_);

  if (!plain_data_.empty()) {
    std::thread thread([&]() { CreateIdManager(); });
    std::thread thread2([&]() { CreateMap(); });
    thread.join();
    thread2.join();
  }
  return plain_data_;
}

void UserDatabase::SetData(const Record &record) {
  std::random_device random;
  std::uniform_int_distribution<uint64_t> dist(0, (sizeof(separators) - 1));
  std::string sql;
  std::string value =
      record.headline + (char)separators[dist(random)] + record.password;

  auto username = map_data_.find(record.username);
  if (username != map_data_.end()) {
    // map contains a key
    username->second.push_back(value);
  } else {
    // map does not contain a key
    sql = "INSERT OR IGNORE INTO Username (username_id) VALUES ('" +
          record.username + "');";
    std::vector<std::string> tmp;
    tmp.push_back(value);
    map_data_[record.username] = tmp;
  }
  sql += "INSERT INTO content (content_id,username_id, data) VALUES (" +
         std::to_string(record.content_id) + ",'" + record.username + "', '" +
         value + "');";
  database_utils::ExecuteSql(connection_ ,sql);
  PrintMap("SetData", map_data_);
  plain_data_.push_back(record);
}

size_t UserDatabase::GetId() {
  std::cout << "plain_data_ len: " << plain_data_.size() << std::endl;
  if (plain_data_.empty()) {
    id_manager_.push_back(1);
    PrintVector("GetId", id_manager_);
    return 1;
  } else {
    if (id_manager_.size() == plain_data_.size()) {
      PrintVector("GetId", id_manager_);
      id_manager_.push_back(1);
      return id_manager_.size();
    }
    PrintVector("GetId", id_manager_);

    for (size_t i = 0; i < id_manager_.size(); ++i) {
      if (id_manager_[i] == 0) {
        ++id_manager_[i];
        return ++i;
      }
    }
    return 0;
  }
}

void UserDatabase::DeleteData(const int &id) {
  std::string sql =
      "DELETE FROM content WHERE content_id = " + std::to_string(id) + ";";
  database_utils::ExecuteSql(connection_, sql);
  if (id == id_manager_.size()) {
    id_manager_.pop_back();
  } else {
    if (id_manager_[id - 1] != 0) {
      --id_manager_[id - 1];
    } else {
      std::cout << "id: " << id << std::endl
                << "id_manager_.size(): " << id_manager_.size() << std::endl
                << "plain_data_.size(): " << plain_data_.size() << std::endl;
      PrintVector("exeption", id_manager_);
      throw std::runtime_error("stack overflow");
    }
  }

  PrintVector("DeleteData", id_manager_);

  std::string username;
  for (int i = 0; i < plain_data_.size(); ++i) {
    if (plain_data_[i].content_id == id) {
      username = plain_data_[i].username;
      plain_data_.erase(plain_data_.begin() + i);
    }
  }
  std::string sql2 = "DELETE FROM username WHERE username_id = '" + username +
                     "' AND NOT EXISTS (SELECT 1 FROM content "
                     "WHERE content.username_id = '" +
                     username + "');";
  database_utils::ExecuteSql(connection_, sql2);
  if (plain_data_.empty()) {
    while (!id_manager_.empty()) {
      id_manager_.pop_back();
    }
  }
  if(map_data_[username].size() <= 1) {
    map_data_.erase(username);
  } else {
    map_data_[username].pop_back();
  }
  PrintMap("DeleteData", map_data_);
}

void UserDatabase::CreateIdManager() {
  size_t max_id = 0;

  if (!plain_data_.empty()) {
    for (auto &i : plain_data_) {
      if (max_id < i.content_id) {
        max_id = i.content_id;
      } else {
        max_id = max_id;
      }
    }
    if (max_id == plain_data_.size()) {
      id_manager_ = std::vector<uint8_t>(max_id, 1);
      PrintVector("CreateIdManager", id_manager_);
    } else if (max_id > plain_data_.size()) {
      id_manager_ = std::vector<uint8_t>(max_id, 0);
      for (auto &i : plain_data_) {
        ++id_manager_[i.content_id - 1];
      }
      PrintVector("CreateIdManager", id_manager_);
    }
  }
}

void UserDatabase::CreateMap() {
  for (auto &i : plain_data_) {
    auto username = map_data_.find(i.username);
    if (username != map_data_.end()) {
      // map contains a key
      username->second.emplace_back("x");
    } else {
      // map does not contain a key
      std::vector<std::string> tmp;
      tmp.emplace_back("x");
      map_data_[i.username] = tmp;
    }
  }
  PrintMap("CreateMap", map_data_);
}

//============== UserDatabase tests functions implementation ===================
#ifdef ENABLE_TESTS
void UserDatabase::SetIdManager(std::vector<uint8_t> id_manager){
    id_manager_ = std::move(id_manager);
}

std::vector<uint8_t> UserDatabase::GetIdManager() {
  return id_manager_;
}

void UserDatabase::SetPlainData(std::vector<Record> plain_data){
    plain_data_ = std::move(plain_data);
}

std::vector<Record> UserDatabase::GetPlainData() {
  return plain_data_;
}
Record UserDatabase::PrepareDataTest(char** text_data) {
  std::vector<Record> tmp;
  PrepareData(&tmp, 0, text_data, nullptr);
  return tmp[0];
}

#endif
//===================== Implement helpful functions ============================
static int PrepareData(void *user_data, int argc, char **argv,
                       char **azColName) {
  auto *data_vector = static_cast<std::vector<Record> *>(user_data);

  Record data;
  if (argv[0]) {
    data.username = argv[0];
  }
  if (argv[1]) {
    data.content_id = std::stoi(argv[1]);
  }
  if (argv[2]) {
    auto to_split = std::string(argv[2]);
    auto separator =
        std::find_first_of(to_split.begin(), to_split.end(),
                           std::begin(separators), std::end(separators));

    data.headline = std::string(to_split.begin(), separator);
    data.password = std::string(separator + 1, to_split.end());
  }

  data_vector->push_back(data);

  return 0;
}

void PrintVector(const char *place, const std::vector<uint8_t> &vec) {
#ifdef DEBUG
  if (vec.empty()) {
    std::cout << place << ": {}";
  } else {
    std::cout << place << ": {";
    for (const auto &value : vec) {
      std::cout << static_cast<int>(value) << ", ";
    }
    std::cout << "\b\b}" << std::endl;
  }
#endif
}

void PrintMap(const char *place, std::map<std::string, std::vector<std::string>> map_data) {
#ifdef DEBUG
  std::cout << "---------------------------" << std::endl << place << " - Print map" << std::endl;
  for (auto & itr : map_data) {
    std::cout << itr.first << "\t\t";
    for(auto &i : itr.second) {
      std::cout << i << ", ";
    }
    std::cout << "\b\b" << std::endl;
  }
  std::cout << "---------------------------" << std::endl;
#endif
}

}  // namespace rain_text
