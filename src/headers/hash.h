#ifndef RAINTEXT_HASH_H
#define RAINTEXT_HASH_H

#include <iostream>
#include <vector>

namespace rain_text::hash {

std::string GetDbName(std::string username);

std::string GetPswdHash(std::string password, std::string username);

std::vector<uint8_t> GetKey(std::string password, std::string username);

}

#endif  // RAINTEXT_HASH_H
