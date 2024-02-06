/* This file was created by Kureii (Tomas Adamek)
 * Date created: 19. 11. 2023
 * This file is under the GPL-3 license
 */
//================================= Includes ===================================
#include "RainText/gui/login_register_manager.hpp"

#include <QChar>
#include <QDebug>
#include <QRegularExpression>
#include <iostream>
#include <memory>

#include "RainText/login.hpp"
#include "RainText/register.hpp"

//================================= Namespace ==================================
namespace rain_text::gui {

//================================= Defines ====================================

#define LOGIN_STATE "loginState"

//================================= Public method ==============================
LoginRegisterManager::LoginRegisterManager(QObject *parent) : QObject(parent) {
  state_ = LOGIN_STATE;
}
bool LoginRegisterManager::CheckFields(const QString &username,
                                       const QString &password,
                                       const QString &password_again) {
  if (state_ != LOGIN_STATE) {
    return username.length() >= 3 && password.length() >= 8 &&
           password == password_again;
  }
  return username.length() >= 3 && password.length() >= 8;
}

void LoginRegisterManager::ConfirmFormUser(const QString &username,
                                           const QString &password) {
  QString path;
  std::vector<uint8_t> key;
  if (state_ == LOGIN_STATE) {
    auto login = std::make_unique<register_login::Login>(username, password);
    if (login->IsLoginSusccessful(path,key)) {
      qDebug("susscess");
      emit loadDb(username);
    } else {
      qDebug("error");
    }
  } else {
    auto registration = std::make_unique<register_login::Register>(username, password);
    if (registration->IsRegisterSusccessful(path)) {
      auto printPath = path.toStdString();
      qDebug(printPath.data());
      emit loadDb(username);
    } else {
      // error
    }
  }
}

void LoginRegisterManager::StateChange(const QString &new_state) {
  state_ = new_state.toStdString();
}

float LoginRegisterManager::PasswordStrength(const QString &password) {
  if (password.isNull() || password.isEmpty()) {
    return 0.0f;
  }
  float result = 0;
  if (password.contains(QRegularExpression("[A-Z]"))) {
    result += .2;
  }
  if (password.contains(QRegularExpression("[a-z]"))) {
    result += .15;
  }
  if (password.contains(QRegularExpression("[0-9]"))) {
    result += .2;
  }
  if (password.contains(
          QRegularExpression("[!@#$%^&*()_+={}\\[\\]:;\"\'<>,.?/|\\\\-]"))) {
    result += .3;
  }
  QRegularExpression pattern("^[A-Z][a-z]*[0-9]+$");
  if (pattern.match(password).hasMatch()) {
    result *= .5;
  }
  result *= (password.length() / 20.0f);

  double score = 0.0;
  QChar lastCharType = '0';
  int typeWeight = 0;

  for (QChar ch : password) {
    if (ch.isLower()) {
      typeWeight = (lastCharType.isLower()) ? 0.35 : 1;
    } else if (ch.isUpper()) {
      typeWeight = (lastCharType.isUpper()) ? 0.5 : 1;
    } else if (ch.isDigit()) {
      typeWeight = (lastCharType.isDigit()) ? 1 : 2;
    } else {
      typeWeight = (lastCharType.isLetterOrNumber()) ? 1.5 : 3;
    }
    score += typeWeight;
    lastCharType = ch;
  }

  double normalizedScore = 0.5 + (score / password.length()) * 0.6;
  if (normalizedScore > 1.1) normalizedScore = 1.1;

  result *= normalizedScore;

  return result <= 1 ? result : 1;
}

//================================= Testing method =============================
#ifdef ENABLE_TESTS

#endif

//================================= Private method =============================

//================================= End namespace ==============================
}  // namespace rain_text::gui
