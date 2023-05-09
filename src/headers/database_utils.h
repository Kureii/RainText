//
// Created by adtom on 08.05.2023.
//

#ifndef RAINTEXT_DATABASE_UTILS_H
#define RAINTEXT_DATABASE_UTILS_H

#include <sqlite3.h>

#include <filesystem>
#include <mutex>
#include <vector>

namespace rain_text::database_utils{



/**
 * @brief Creates a directory if it does not already exist.
 *
 * This function checks if the given directory exists, and if not, it tries to
 * create the directory. If the directory cannot be created, an exception is thrown.
 *
 * @param dir A reference to the std::filesystem::path object representing the directory to be created.
 * @throws std::runtime_error if the directory could not be created.
 */
void CreateDir (std::filesystem::path &dir);

/**
 * @brief Opens a SQLite database.
 *
 * This function opens a SQLite database given its file path. If there is an error
 * while opening the database, an exception is thrown.
 *
 * @param path A pointer to a C-style string representing the file path of the database.
 * @param connection A reference to a sqlite3 pointer that will hold the connection to the opened database.
 * @throws std::runtime_error if the database could not be opened.
*/
void OpenDb (const char * path, sqlite3 *& connection);


void ExecuteSql(sqlite3* connection, const std::string& sql, int (*callback)(void*, int, char**, char**)= nullptr, void* data= nullptr);

}

#endif  // RAINTEXT_DATABASE_UTILS_H
