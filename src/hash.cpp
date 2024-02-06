/* This file was created by Kureii (Tomas Adamek)
 * Date created: 30. 01. 2024
 * This file is under the GPL-3 license
 */
//================================= Includes ===================================
#include "RainText/hash.hpp"

#include <argon2.h>
#include <libscrypt.h>
#include <md5.h>
#include <sha3.h>
#include <chrono>

#include <iomanip>

//================================= Namespace ==================================
namespace rain_text::hash {
//================================= Public method ==============================

QString Hash::GetDbName(QString username) {
  char symbols[37] = "abcdefghijklmnopqrstuvwxyz0123456789";
  uint8_t digest[16];  // output
  while (username.size() < 64) {
    username += username;
  }
  std::string usernameStd = username.toStdString();
  const uint8_t* initial_msg =
      reinterpret_cast<const uint8_t*>(usernameStd.c_str());
  size_t initial_len = username.size();
  md5::md5(initial_msg, initial_len, digest);
  QString result;
  for (int i = 0; i < 16; i = i + 2) {
    uint8_t tmp = (digest[i] + digest[i + 1]) % 37;
    result += symbols[tmp];
  }
  return result;
}

QByteArray Hash::GetPswdHash(QString password, QString username) {
  uint8_t salt[64];
  sha3_HashBuffer(512, SHA3_FLAGS_NONE, password.data(), password.size(), salt,
                  64);
  uint8_t password_hash[64];
  std::string data = password.toStdString() + "\x1E" + username.toStdString();
  libscrypt_scrypt(reinterpret_cast<const uint8_t*>(data.data()), data.size(),
                   salt, 64, 65536, 8, 1, password_hash, 64);
  return {reinterpret_cast<char*>(password_hash), 64};
}
std::vector<uint8_t> Hash::GetKey(QString password, QString username) {
  uint8_t pre_salt[64];
  sha3_HashBuffer(512, SHA3_FLAGS_NONE, username.data(), username.size(),
                  pre_salt, 64);

  uint8_t salt[64];
  std::string data = username.toStdString() + "\x1C" + password.toStdString();
  uint8_t pre_result[1024];
  libscrypt_scrypt(reinterpret_cast<const uint8_t*>(data.data()), data.size(),
                   pre_salt, 64, 131072, 16, 1, salt, 64);
  argon2id_hash_raw(7, 1 << 18, 2, password.data(), password.size(), salt, 64,
                    pre_result, 1024);
  std::vector<uint8_t> result;
  for (auto& i : pre_result) {
    result.emplace_back(i);
  }
  return result;
}

QString Hash::uint8_array_to_hex_string(const uint8_t* data, size_t length) {
  std::stringstream stringstream;
  stringstream << std::hex << std::setfill('0');
  for (size_t i = 0; i < length; ++i) {
    stringstream << std::setw(2) << static_cast<int>(data[i]);
  }
  return QString(stringstream.str().c_str());
}

//================================= Testing method =============================
#ifdef ENABLE_TESTS

#endif

//================================= Private method =============================

//================================= End namespace ==============================
}  // namespace rain_text::register_login