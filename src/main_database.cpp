
//================================= Includes ===================================
#include "headers/main_database.h"

#include "headers/database_utils.h"
#include "headers/hash.h"

//================================= Namespace ==================================
namespace rain_text {
//======================== Define helpful variables ============================

//======================= Define helpful structures ============================

//======================= Define helpful functions =============================
static int GetDatabaseHash(void *user_data, int argc, char **argv, char **azColName);

//====================== MainDatabase implementation ===========================
sqlite3* MainDatabase::connection_ = nullptr;
MainDatabase::MainDatabase(std::string path) {
  std::filesystem::path db_path(path);
  std::filesystem::path db_dir = db_path.parent_path();

  database_utils::CreateDir(db_dir);

  database_utils::OpenDb(path.c_str(), connection_);

  auto sql = std::string(
      "CREATE TABLE IF NOT EXISTS users("
      "hash TEXT UNIQUE NOT NULL,"
      "PRIMARY KEY (hash)"
      ");");

  database_utils::ExecuteSql(connection_, sql);
}

MainDatabase::~MainDatabase() {}

std::string MainDatabase::CreateUser(std::list<std::string> list) {
  auto username = list.front();
  auto password = list.back();
  auto db_name = hash::GetDbName(username);
  auto password_hash = hash::GetPswdHash(password, username);

  std::string path = "./database/" + db_name + ".db";

  std::string err;
  if (std::filesystem::exists(path)) {
    return err;
  }

  std::string sql =
      "INSERT OR IGNORE INTO users (hash) VALUES ('" + password_hash + "');";
  database_utils::ExecuteSql(connection_, sql);
  return path;
}

std::string MainDatabase::LoginUser(std::list<std::string> list) {
  auto username = list.front();
  auto password = list.back();
  auto db_name = hash::GetDbName(username);
  auto password_hash = hash::GetPswdHash(password, username);

  std::string path = "./database/" + db_name + ".db";

  std::string err;

  std::string db_hash;
  std::string sql = "SELECT hash FROM users WHERE hash = '" + password_hash + "';";
  database_utils::ExecuteSql(connection_, sql, GetDatabaseHash, &db_hash);
  if (db_hash.empty()) {
    return err;
  } else {
    return path;
  }
}
//============== MainDatabase tests functions implementation ===================
#ifdef ENABLE_TESTS

#endif

//===================== Implement helpful functions ============================
static int GetDatabaseHash(void *user_data, int argc, char **argv, char **azColName) {
  std::string *hash_output = static_cast<std::string *>(user_data);

  for (int i = 0; i < argc; i++) {
    std::string column_name(azColName[i]);
    std::string value(argv[i] ? argv[i] : "NULL");

    if (column_name == "hash") {
      *hash_output = value;
      break;
    }
  }

  return 0;
}

}  // namespace rain_text