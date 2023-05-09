
//================================= Includes ===================================
#include "headers/main_database.h"
#include "headers/database_utils.h"
#include "headers/hash.h"

//================================= Namespace ==================================
namespace rain_text {
//======================== Define helpful variables ============================

//======================= Define helpful structures ============================

//======================= Define helpful functions =============================

//====================== MainDatabase implementation ===========================
sqlite3 * MainDatabase::connection_= nullptr;
MainDatabase::MainDatabase(std::string path) {
  std::filesystem::path db_path(path);
  std::filesystem::path db_dir = db_path.parent_path();

  database_utils::CreateDir(db_dir);

  database_utils::OpenDb(path.c_str(), connection_);

  auto sql = std::string(
      "CREATE TABLE IF NOT EXISTS users("
      "username_id TEXT UNIQUE NOT NULL,"
      "hash TEXT NOT NULL,"
      "path TEXT NOT NULL,"
      "PRIMARY KEY (username_id)"
      ");");

  database_utils::ExecuteSql(connection_ ,sql);

}

MainDatabase::~MainDatabase() {}

std::string MainDatabase::CreateUser(std::list<std::string> list) {
  auto username = list.front();
  auto password = list.back();
  auto db_name = hash::GetDbName(username);
  std::cout << db_name << std::endl;
  std::cout << hash::GetPswdHash(password) << std::endl;
  // TODO: Hash class
  //std::string myhash = password;
  std::string path  = "./database/" + db_name + ".db";

  /*std::cout << "username:\t" << username << std::endl;
  std::cout << "password:\t" << password << std::endl;
  std::string sql = "INSERT INTO users (username_id,hash, path) VALUES (" + username + ",'" + myhash + "', '" + path + "');";
  database_utils::ExecuteSql(connection_ ,sql);*/
  return path;
}
//============== MainDatabase tests functions implementation ===================
#ifdef ENABLE_TESTS

#endif

//===================== Implement helpful functions ============================

}  // namespace rain_text