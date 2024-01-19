/* This file was created by Kureii (Tomas Adamek)
 * Date created: 19. 11. 2023
 * This file is under the MIT license
 */

#ifndef RAINTEXT_LOGIN_REGISTER_MANAGER_H
#define RAINTEXT_LOGIN_REGISTER_MANAGER_H

#include <QObject>
namespace rain_text::gui {

class LoginRegisterManager : public QObject {
  Q_OBJECT
 public:
  explicit LoginRegisterManager(QObject *parent = nullptr);

  Q_INVOKABLE void ConfirmFormUser(const QString &username,
                                const QString &password);

  Q_INVOKABLE void StateChange(const QString &new_state);

  Q_INVOKABLE bool CheckFields(const QString &username,
                               const QString &password,
                               const QString &password_again);

  Q_INVOKABLE float PasswordStrength(const QString &password);
 private:
  std::string state_;
signals:
 void registrationComplete();
 void loginComplete();
 void errorOccurred(const QString &error);

};

}  // namespace rain_text::gui

#endif  // RAINTEXT_LOGIN_REGISTER_MANAGER_H
