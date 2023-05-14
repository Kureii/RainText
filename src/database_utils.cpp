//================================= Includes ===================================
#include "headers/database_utils.h"

#include <exception>
#include <mutex>
//================================= Namespace ==================================
namespace rain_text::database_utils {
//======================== Define helpful variables ============================
std::mutex mutex;
//======================= Define helpful structures ============================

//======================= Define helpful functions =============================

//===================== database_utils implementation ==========================
void CreateDir(std::filesystem::path& dir) {
  if (!std::filesystem::exists(dir)) {
    if (!std::filesystem::create_directory(dir)) {
      throw std::runtime_error(
          std::string("Cannot create directory: " + dir.string()));
    }
  }
}

void OpenDb(const char* path, sqlite3*& connection) {
  std::unique_lock<std::mutex> lock(mutex);
  if (sqlite3_open(path, &connection) != SQLITE_OK) {
    sqlite3_close(connection);
    throw std::runtime_error(std::string(
        "Cannot open database: " + std::string(sqlite3_errmsg(connection))));
  }
  lock.unlock();
}

void ExecuteSql(sqlite3* connection, const std::string& sql,
                int (*callback)(void*, int, char**, char**), void* data) {
  std::unique_lock<std::mutex> lock(mutex);
  char* err_msg = nullptr;
  if (sqlite3_exec(connection, sql.c_str(), callback, data, &err_msg) !=
      SQLITE_OK) {
    std::string error_message = "SQL error: ";
    if (err_msg) {
      error_message += err_msg;
      sqlite3_free(err_msg);
    } else {
      error_message += "Unknown error";
    }
    sqlite3_close(connection);
    throw std::runtime_error(error_message);
  }
  lock.unlock();
}
//============== database_utils tests functions implementation =================
#ifdef ENABLE_TESTS

#endif

//===================== Implement helpful functions ============================

}  // namespace rain_text::database_utils