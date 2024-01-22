/* This file was created by Kureii (Tomas Adamek)
 * Date created: 19. 01. 2024
 * This file is under the GPL-3 license
 */
//================================= Includes ===================================

#include "RainText/gui/generate_password.h"

#include <random>

//================================= Namespace ==================================
namespace rain_text::gui {

//================================= Public method ==============================
GeneratePassword::GeneratePassword(QObject* parent) : QObject(parent) {}

void GeneratePassword::ClickToGenerateButton(bool majusculeGenPass,
                                             bool minusculeGenPass,
                                             bool numberGenPass,
                                             bool specialGenPass, int length) {
  std::string passwordChars;
  if (majusculeGenPass) {
    passwordChars += "QWERTZUIOPASDFGHJKLYXCVBNM";
  }

  if (minusculeGenPass) {
    passwordChars += "qwertzuiopasdfghjklyxcvbnm";
  }

  if (numberGenPass) {
    passwordChars += "0123456789";
  }

  if (specialGenPass) {
    passwordChars += R"(!@#$%^&*()_+={}\[\]:;"'<>,.?/|\-)";
  }

  std::string password;
  std::mt19937 generator(static_cast<unsigned int>(time(nullptr)));
  std::uniform_int_distribution<> distribution(0, passwordChars.size() - 1);

  for (int i = 0; i < length; ++i) {
    password += passwordChars[distribution(generator)];
  }

  emit passwordGenerated(QString::fromStdString(password));
}

//================================= Testing method =============================
#ifdef ENABLE_TESTS

#endif

//================================= Private method =============================

//================================= End namespace ==============================
}  // namespace rain_text::gui