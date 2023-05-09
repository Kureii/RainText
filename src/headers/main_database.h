
#ifndef RAINTEXT_MAIN_DATABASE_H
#define RAINTEXT_MAIN_DATABASE_H

#include <iostream>
#include <vector>
#include <list>
#include <sqlite3.h>

namespace rain_text {

class MainDatabase {
public:
 static sqlite3 * connection_;

 MainDatabase(std::string path);
 ~MainDatabase();
 std::string CreateUser(std::list<std::string> list);
};
}
#endif  // RAINTEXT_MAIN_DATABASE_H
