//================================= Includes ===================================
#include "headers/hash.h"

#include <argon2.h>
#include <libscrypt.h>
#include <md5.h>
#include <sha3.h>
#include <chrono>

#include <iomanip>

//================================= Namespace ==================================
namespace rain_text::hash {
//======================== Define helpful variables ============================
char symbols[37] = "abcdefghijklmnopqrstuvwxyz0123456789";

//======================= Define helpful structures ============================

//======================= Define helpful functions =============================
std::string uint8_array_to_hex_string(const uint8_t* data, size_t length);

//========================== hash implementation ===============================
std::string GetDbName(std::string username) {
  uint8_t digest[16];  // output
  while (username.size() < 64) {
    username += username;
  }
  const uint8_t* initial_msg =
      reinterpret_cast<const uint8_t*>(username.c_str());
  size_t initial_len = username.size();
  md5::md5(initial_msg, initial_len, digest);
  std::string result;
  for (int i = 0; i < 16; i = i + 2) {
    uint8_t tmp = (digest[i] + digest[i + 1]) % 37;
    result += symbols[tmp];
  }
  return result;
}

std::string GetPswdHash(std::string password, std::string username) {
  auto start_time = std::chrono::high_resolution_clock::now();
  uint8_t salt[64];
  sha3_HashBuffer(512, SHA3_FLAGS_NONE, password.data(), password.size(), salt,
                  64);
  uint8_t password_hash[64];
  std::string data = password + "\x1E" + username;
  libscrypt_scrypt(reinterpret_cast<const uint8_t*>(data.data()), data.size(),
                   salt, 64, 65536, 8, 1, password_hash, 64);
  return uint8_array_to_hex_string(password_hash, 64);
}

std::vector<uint8_t> GetKey(std::string password, std::string username) {
  uint8_t pre_salt[64];
  sha3_HashBuffer(512, SHA3_FLAGS_NONE, username.data(), username.size(),
                  pre_salt, 64);

  uint8_t salt[64];
  std::string data = username + "\x1C" + password;
  uint8_t pre_result[256];
  libscrypt_scrypt(reinterpret_cast<const uint8_t*>(data.data()), data.size(),
                   pre_salt, 64, 131072, 16, 1, salt, 64);
  argon2id_hash_raw(7, 1 << 18, 2, password.data(), password.size(), salt, 64,
                    pre_result, 256);
  std::vector<uint8_t> result;
  for (auto& i : pre_result) {
    result.emplace_back(i);
  }
  return result;
}
//=================== hash tests functions implementation ======================
#ifdef ENABLE_TESTS

#endif

//===================== Implement helpful functions ============================
std::string uint8_array_to_hex_string(const uint8_t* data, size_t length) {
  std::stringstream stringstream;
  stringstream << std::hex << std::setfill('0');
  for (size_t i = 0; i < length; ++i) {
    stringstream << std::setw(2) << static_cast<int>(data[i]);
  }
  return stringstream.str();
}
}  // namespace rain_text::hash