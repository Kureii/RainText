//================================= Includes ===================================
#include "headers/user_database.h"

#include "core/rain_text_core.h"
#include <algorithm>
#include <atomic>
#include <condition_variable>
#include <cstring>
#include <mutex>
#include <random>
#include <string>
#include <thread>
#include <utility>

#include "headers/database_utils.h"

//================================= Namespace ==================================
namespace rain_text {
//======================== Define helpful variables ============================
uint8_t separators[] = {0x1C, 0x1D, 0x1E, 0x1F};
std::mutex encrypt_mutex;
std::condition_variable encrypt_cond_var;
//======================= Define helpful structures ============================
//======================= Define helpful functions =============================
static int PrepareData(void *user_data, int argc, char **argv,
                       char **azColName);

void PrintVector(const char *place, const std::vector<uint8_t> &vec);
void PrintMap(const char *place,
              std::map<std::string, std::vector<std::string>> map_data);

std::string uint8ToHexString(const std::vector<uint8_t>& vec);
std::vector<uint8_t> hexStringToVector(const std::string& hexStr);

//====================== UserDatabase implementation ===========================
sqlite3 *UserDatabase::connection_ = nullptr;
bool UserDatabase::gui_stopped_ = false;
bool UserDatabase::add_item_ = false;

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
      "content_id INTEGER PRIMARY KEY,"
      "username_id TEXT NOT NULL,"
      "data TEXT,"
      "FOREIGN KEY (username_id) REFERENCES Username (username_id) ON DELETE "
      "CASCADE"
      ");");

  database_utils::ExecuteSql(connection_, sql);
#ifdef ENABLE_TESTS
  db_path_ = db_dir;
#endif
}

UserDatabase::~UserDatabase() {
  sqlite3_close(connection_);
#ifdef ENABLE_TESTS
  try {
    std::filesystem::remove_all(db_path_);
  } catch (const std::filesystem::filesystem_error &e) {
    std::cerr << "Error deleting path and contents: " << e.what() << std::endl;
  }
#endif
}

std::vector<EncryptRecord> UserDatabase::GetData() {
  std::string sql = "SELECT username_id, content_id, data FROM content;";

  database_utils::ExecuteSql(connection_, sql, PrepareData, &encrypted_data_);

  return encrypted_data_;
}

void UserDatabase::SetData(const Record &record) {
  std::cout<< "\tSetData"<<std::endl;
  plain_data_.push_back(record);
  for(auto &i : plain_data_) {
    std::cout << "content_id:\t" << i.content_id << std::endl;
    std::cout << "username:\t" << i.username << std::endl;
    std::cout << "password:\t" << i.password << std::endl;
    std::cout << "------------------------------------" << std::endl;
  }
  std::this_thread::sleep_for(std::chrono::microseconds (500));
  this->SetAddItem(true);
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

  std::string username;
  for (int i = 0; i < plain_data_.size(); ++i) {
    if (plain_data_[i].content_id == id) {
      username = plain_data_[i].username;
      plain_data_.erase(plain_data_.begin() + i);
    }
  }
  if (plain_data_.empty()) {
    while (!id_manager_.empty()) {
      id_manager_.pop_back();
    }
  }
  this->SetAddItem(true);
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
std::vector<Record> UserDatabase::DecryptData(std::vector<uint8_t> key) {
  plain_data_ = std::vector<Record>();
  key_ = std::move(key);
  for (auto &i : encrypted_data_) {
    Record tmp;
    tmp.content_id = i.content_id;

    // get username
    std::vector<uint8_t> encrypted_username_uint8_t(i.username.begin(),
                                                    i.username.end());
    auto rtc = std::make_unique<rain_text_core::RainTextCore>(
        100, key_, encrypted_username_uint8_t);
    std::vector<uint8_t> decrypted_username_uint8_t;
    rtc->Decrypt(decrypted_username_uint8_t);
    tmp.username = std::string(decrypted_username_uint8_t.begin(),
                               decrypted_username_uint8_t.end());

    // get headline and password
    rtc->SetText(i.data);
    std::vector<uint8_t> decrypted_data_uint8_t;
    rtc->Decrypt(decrypted_data_uint8_t);
    auto separator = std::find_if(
        decrypted_data_uint8_t.begin(), decrypted_data_uint8_t.end(),
        [](uint8_t value) {
          return std::find(std::begin(separators), std::end(separators),
                           value) != std::end(separators);
        });
    if (separator != decrypted_data_uint8_t.end()) {
      // separate headline and password
      std::vector<uint8_t> headline_uint8_t(decrypted_data_uint8_t.begin(),
                                            separator);
      std::vector<uint8_t> password_uint8_t(separator + 1,
                                            decrypted_data_uint8_t.end());
      tmp.headline =
          std::string(headline_uint8_t.begin(), headline_uint8_t.end());
      tmp.password =
          std::string(password_uint8_t.begin(), password_uint8_t.end());
    }
    plain_data_.emplace_back(tmp);
  }
  // plain_data_ = encrypted_data_;
  std::thread enrolment_manager([this]() {
    EnrolmentManager(this);
  });
  enrolment_manager.detach();
  return plain_data_;
}

const std::vector<Record> &UserDatabase::GetPlainData() const {
  std::cout<< "\tGetPlainData"<<std::endl;
  for(auto &i : plain_data_) {
    std::cout << "content_id:\t" << i.content_id << std::endl;
    std::cout << "username:\t" << i.username << std::endl;
    std::cout << "password:\t" << i.password << std::endl;
    std::cout << "------------------------------------" << std::endl;
  }
  return plain_data_;
}
void UserDatabase::SetGuiStopped(bool guiStopped) {
  {
    std::unique_lock<std::mutex> lock(encrypt_mutex);
    gui_stopped_ = guiStopped;
  }
  encrypt_cond_var.notify_one();
}
void UserDatabase::SetAddItem(bool addItem) {
  {
    std::unique_lock<std::mutex> lock(encrypt_mutex);
    add_item_ = addItem;
  }
  encrypt_cond_var.notify_one();
}

void UserDatabase::EnrolmentManager(
    UserDatabase * user_db) {  // Thread
  thread_local auto plain_data = user_db->GetPlainData();
  // random for separators
  thread_local std::random_device random;
  thread_local std::uniform_int_distribution<uint64_t> dist(
      0, (sizeof(separators) - 1));
  // as long as the gui is running, there will be a thread
  while (!UserDatabase::gui_stopped_) {
    std::unique_lock<std::mutex> lock(encrypt_mutex);
    // waiting for variable change
    encrypt_cond_var.wait(lock, [&user_db] {
      return user_db->add_item_ || user_db->gui_stopped_;
    });

    add_item_ = false;
    plain_data = user_db->GetPlainData();

    std::cout<< "\tEnrolmentManager"<<std::endl;
    for(auto &i : plain_data) {
      std::cout << "content_id:\t" << i.content_id << std::endl;
      std::cout << "username:\t" << i.username << std::endl;
      std::cout << "password:\t" << i.password << std::endl;
      std::cout << "------------------------------------" << std::endl;
    }

    std::map<std::string, std::map<size_t, std::string>> map_data;
    std::map<std::string, std::map<size_t, std::string>>
        map_encrypt;
    for (auto &i : plain_data) {
      auto username = map_data.find(i.username);
      if (username != map_data.end()) {
        // map contains a key
        username->second[i.content_id] =
            i.headline + (char)separators[dist(random)] + i.password;
      } else {
        // map does not contain a key
        std::map<size_t, std::string> tmp_map;
        tmp_map[i.content_id] =
            i.headline + (char)separators[dist(random)] + i.password;
        map_data[i.username] = tmp_map;
      }
    }

    // remove all from tables
    std::string delete_sql = "DELETE FROM content; DELETE FROM username;";
    database_utils::ExecuteSql(user_db->connection_, delete_sql);

    //thread_local std::vector<std::string> sql_seq;
    // obtaining the data sequence
   std::string sql;
    for (auto &i : map_data) {  // first username_id, second connect_id and data
       std::vector<uint8_t> plain_username_uint8_t(i.first.begin(),
                                                               i.first.end());
       auto rtc = std::make_unique<rain_text_core::RainTextCore>(
          16, user_db->key_, plain_username_uint8_t);
       std::vector<uint8_t> encrypt_username_vector;
      rtc->Encrypt(encrypt_username_vector);
       auto encrypt_username = uint8ToHexString(
          encrypt_username_vector);
      sql += "INSERT OR IGNORE INTO Username (username_id) VALUES ('";
      sql += encrypt_username;
      sql += "');\n";
      for (auto &j : i.second) {  // first connect_id, second data
         std::vector<uint8_t> plain_data_uint8_t(j.second.begin(),
                                                             j.second.end());
        rtc->SetText(plain_data_uint8_t);
         std::vector<uint8_t> encrypted_data_vector;
        rtc->Encrypt(encrypted_data_vector);
         std::string encrypted_data_string = uint8ToHexString(
            encrypted_data_vector);
        sql += "INSERT INTO content (content_id,username_id, data) VALUES (";
        sql += std::to_string(j.first);
        sql += ",'";
        sql += encrypt_username;
        sql += "', '";
        sql += encrypted_data_string;
        sql += "');\n";
      }
    }

    if (!sql.empty()) {
      // fill tables
      //for (auto &s : sql_seq) {
        std::cout << sql << std::endl << std::endl << std::endl;
        database_utils::ExecuteSql(user_db->connection_, sql);
      //}
    }

    plain_data.clear();
  }
  std::cout << "thread end" << std::endl;
  delete user_db;
}
//============== UserDatabase tests functions implementation ===================
#ifdef ENABLE_TESTS
void UserDatabase::SetIdManager(std::vector<uint8_t> id_manager) {
  id_manager_ = std::move(id_manager);
}

std::vector<uint8_t> UserDatabase::GetIdManager() { return id_manager_; }

void UserDatabase::SetPlainData(std::vector<Record> plain_data) {
  plain_data_ = std::move(plain_data);
}

Record UserDatabase::PrepareDataTest(char **text_data) {
  std::vector<Record> tmp;
  PrepareData(&tmp, 0, text_data, nullptr);
  return tmp[0];
}

#endif
//===================== Implement helpful functions ============================
static int PrepareData(void *user_data, int argc, char **argv,
                       char **azColName) {
  auto *data_vector = static_cast<std::vector<EncryptRecord> *>(user_data);

  EncryptRecord data;
  if (argv[0]) {
    std::size_t length = std::strlen(argv[0]);
    auto tmp = std::string(argv[0], argv[0] + length);
    data.username = hexStringToVector(tmp);
  }
  if (argv[1]) {
    data.content_id = std::stoi(argv[1]);
  }
  if (argv[2]) {
    std::size_t length = std::strlen(argv[2]);
    auto tmp = std::string(argv[2], argv[2] + length);
    data.data = hexStringToVector(tmp);
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

void PrintMap(const char *place,
              std::map<std::string, std::vector<std::string>> map_data) {
#ifdef DEBUG
  std::cout << "---------------------------" << std::endl
            << place << " - Print map" << std::endl;
  for (auto &itr : map_data) {
    std::cout << itr.first << "\t\t";
    for (auto &i : itr.second) {
      std::cout << i << ", ";
    }
    std::cout << "\b\b" << std::endl;
  }
  std::cout << "---------------------------" << std::endl;
#endif
}

std::string uint8ToHexString(const std::vector<uint8_t>& vec) {
  std::stringstream ss;
  ss << std::hex << std::setfill('0');
  for (uint8_t value : vec) {
    ss << std::setw(2) << static_cast<int>(value);
  }
  return ss.str();
}

std::vector<uint8_t> hexStringToVector(const std::string& hexStr) {
  std::vector<uint8_t> result;
  size_t length = hexStr.length();
  unsigned int intValue;

  for (size_t i = 0; i < length; i += 2) {
    std::stringstream ss(hexStr.substr(i, 2));
    ss >> std::hex >> intValue;
    result.push_back(static_cast<uint8_t>(intValue));
  }

  return result;
}

}  // namespace rain_text
