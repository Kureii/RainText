#include "headers/pass_gen.h"

#include <random>

namespace rain_text::pass_gen {

std::string GeneratePassword(uint8_t len) {
  std::random_device random;
  static const char characters[] =
      "abcdefghijklmnopqrstuvwxyz"
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      "0123456789"
      "!@#$%^&*()";

  std::string password;
  for (int i = 0; i < len; ++i) {
    std::uniform_int_distribution<int> dist(0, (sizeof(characters) - 1));
    auto c = characters[dist(random)];
    password += c;
  }

  return password;
}
}  // namespace rain_text::pass_gen
