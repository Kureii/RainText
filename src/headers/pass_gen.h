#ifndef RAINTEXT_PASS_GEN_H
#define RAINTEXT_PASS_GEN_H

#include <iostream>
namespace rain_text::pass_gen {
std::string GeneratePassword(uint8_t len = 16);
}

#endif  // RAINTEXT_PASS_GEN_H
