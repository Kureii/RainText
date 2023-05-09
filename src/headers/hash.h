#ifndef RAINTEXT_HASH_H
#define RAINTEXT_HASH_H

#include <iostream>

namespace rain_text::hash {

std::string GetDbName(std::string username);

std::string GetPswdHash(std::string password);

std::string GetKey(std::string password);

}

#endif  // RAINTEXT_HASH_H
