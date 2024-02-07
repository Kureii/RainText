/* This file was created by Kureii (Tomas Adamek)
 * Date created: 19. 11. 2023
 * This file is under the GPL-3 license
 */
//================================= Includes ===================================
#include "RainText/gui/login_register_manager.hpp"

#include <QChar>
#include <QDebug>
#include <QFuture>
#include <QRegularExpression>
#include <QtConcurrent/QtConcurrent>
#include <chrono>
#include <iostream>
#include <memory>
#include <thread>

#include "RainText/encrypt_decrypt.hpp"
#include "RainText/login.hpp"
#include "RainText/register.hpp"
#include "RainText/user_db.hpp"

//================================= Namespace ==================================
namespace rain_text::gui {

//================================= Defines ====================================

#define LOGIN_STATE "loginState"

//================================= Public method ==============================
LoginRegisterManager::LoginRegisterManager(QObject *parent)
    : QObject(parent), m_recordListModel_(new model::RecordListModel(this)) {
  state_ = LOGIN_STATE;
  connect(this, &LoginRegisterManager::newItem, this,
          &LoginRegisterManager::handleNewItem);
  QThreadPool::globalInstance()->setMaxThreadCount(MAX_THREADS);
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
  connect(&watcher_, &QFutureWatcher<void>::finished, this,
          &LoginRegisterManager::onAsyncOperationFinished);
  QString path;
  if (state_ == LOGIN_STATE) {
    auto login = std::make_unique<register_login::Login>(username, password);
    if (login->IsLoginSusccessful(path)) {
      emit loadDb(username);
      QFuture<void> future =
          QtConcurrent::run([this, path, login = std::move(login)]() mutable {
            QString mes = "Generate key";
            float progress = 0.01f;
            emit updateLoadDbProgress(progress, mes);

            // Get key and database records
            auto key = login->GetKey();
            auto userDb = std::make_unique<UserDb>(path);
            auto records = userDb->GetAllRecords();

            if (records.empty()) {
              return;
            }

            auto updateProgressAndEmitItem = [this](const RecordItem &item,
                                                    int index, int total) {
              float progress = static_cast<float>(index + 1) / total;
              emit updateLoadDbProgress(progress,
                                        item.headlineText + " decrypted");
              emit newItem(item);
            };

            size_t totalRecords = records.size();
            float progressIncrement = 1.0f / totalRecords;

            QVector<QFutureWatcher<RecordItem> *> watchers;
            watchers.reserve(totalRecords);

            for (int i = 0; i < records.size(); ++i) {
              auto watcher = new QFutureWatcher<RecordItem>(this);
              connect(watcher, &QFutureWatcher<RecordItem>::finished, this,
                      [watcher, updateProgressAndEmitItem, i, totalRecords]() {
                        auto item = watcher->result();
                        updateProgressAndEmitItem(item, i, totalRecords);
                        watcher->deleteLater();
                      });

              QFuture<RecordItem> future = QtConcurrent::run([=]() {
                auto record = records[i];
                return EncryptDecrypt::DecryptRecordItem(record, key);
              });
              watcher->setFuture(future);
              watchers.append(watcher);
            }
          });
      watcher_.setFuture(future);
    } else {
      qDebug("error");
    }
  } else {
    auto registration =
        std::make_unique<register_login::Register>(username, password);
    if (registration->IsRegisterSusccessful(path)) {
      auto printPath = path.toStdString();
      qDebug(printPath.data());
      emit loadDb(username);
      /*auto userDb = UserDb(key, path);
      progress = 1;
      mes = "Done";
      emit updateLoadDbProgress(progress, mes);*/
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
//================================= Public slots ===============================
void LoginRegisterManager::onAsyncOperationFinished() {
  QString msg = QString("Done");
  float progress = 1.0f;
  qDebug("Done message");
  emit updateLoadDbProgress(progress, msg);
}

void LoginRegisterManager::handleNewItem(RecordItem item) {
  m_recordListModel_->addRecordItem(item);
}

//================================= Testing method =============================
#ifdef ENABLE_TESTS

#endif

//================================= Private method =============================

//================================= End namespace ==============================
}  // namespace rain_text::gui
