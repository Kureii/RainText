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
#include <utility>

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
    : QObject(parent),
      m_recordListModel_(new model::RecordListModel(this)),
      enrollmentManagerThread(new QThread(this)),
      enrollmentManager(nullptr) {
  state_ = LOGIN_STATE;
  connect(this, &LoginRegisterManager::newItem, this,
          &LoginRegisterManager::handleNewItem);
  connect(this, &LoginRegisterManager::setNewKey, this,
          &LoginRegisterManager::onSetKey);
  connect(qApp, &QCoreApplication::aboutToQuit, this,
          &LoginRegisterManager::onAppQuit);
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
      path_ = path;
      emit loadDb(username);
      auto future = getKey(path, std::move(login));
      watcher_.setFuture(future);
    } else {
      QString msg = "";
      emit loginError(msg);
    }
  } else {
    auto registration =
        std::make_unique<register_login::Register>(username, password);
    if (registration->IsRegisterSusccessful(path)) {
      auto printPath = path.toStdString();
      emit loadDb(username);
      /*auto userDb = UserDb(key, path);
      progress = 1;
      mes = "Done";
      emit updateLoadDbProgress(progress, mes);*/
    } else {
      QString msg = "";
      emit registerError(msg);
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
  emit updateLoadDbProgress(progress, msg);
  startEnrollmentThread();
}

void LoginRegisterManager::handleNewItem(RecordItem item) {
  m_recordListModel_->addRecordItem(item);
}

void LoginRegisterManager::onSetKey(std::vector<uint8_t> key) {
  key_ = std::move(key);
}

void LoginRegisterManager::onAppQuit() {
  emit terminateThread(m_recordListModel_->GetList(), iterations_);
  enrollmentManagerThread->quit();
  enrollmentManagerThread->wait();
}

//================================= Testing method =============================
#ifdef ENABLE_TESTS

#endif

//================================= Private method =============================
void LoginRegisterManager::startEnrollmentThread() {
  auto key = key_;
  auto path = path_;
  enrollmentManager = new EnrollmentManager(path, key);
  enrollmentManager->moveToThread(enrollmentManagerThread);

  auto success = connect(enrollmentManagerThread, &QThread::started,
                         enrollmentManager, &EnrollmentManager::run);
  if (!success) {
    qWarning() << "Error: connect(enrollmentManagerThread, &QThread::started, "
                  "enrollmentManager, &EnrollmentManager::run)";
  }

  success =
      connect(this, &LoginRegisterManager::terminateThread, enrollmentManager,
              &EnrollmentManager::terminate, Qt::DirectConnection);
  if (!success) {
    qWarning() << "Error: connect(this, "
                  "&LoginRegisterManager::terminateThread, enrollmentManager, "
                  "&EnrollmentManager::terminate, Qt::DirectConnection)";
  }

  success = connect(enrollmentManagerThread, &QThread::finished,
                    enrollmentManager, &QObject::deleteLater);
  if (!success) {
    qWarning() << "Error: connect(enrollmentManagerThread, &QThread::finished, "
                  "enrollmentManager, &QObject::deleteLater)";
  }

  success =
      connect(enrollmentManager, &EnrollmentManager::finished,
              m_recordListModel_, &model::RecordListModel::savingFinished);
  if (!success) {
    qWarning()
        << "Error: connect(enrollmentManager, &EnrollmentManager::finished, "
           "m_recordListModel_, &model::RecordListModel::savingFinished)";
  }

  connect(
      m_recordListModel_, &model::RecordListModel::saveChanges, this,
      [this](const QList<RecordItem> &listModel, int iterations) {
        qDebug() << "Lambda function received saveChanges signal.";
        enrollmentManager->addRecords(listModel, iterations);
      },
      Qt::QueuedConnection);

  /*success = connect(m_recordListModel_, &model::RecordListModel::saveChanges,
  enrollmentManager, &EnrollmentManager::addRecords); if (!success) { qWarning()
  << "Error: connect(m_recordListModel_, &model::RecordListModel::saveChanges,
  enrollmentManager, &EnrollmentManager::addRecords)";
  }*/

  enrollmentManagerThread->start();

  qDebug() << "RecordListModel thread:" << m_recordListModel_->thread();
  qDebug() << "EnrollmentManager thread:" << enrollmentManager->thread();
  qDebug() << "Current thread:" << QThread::currentThread();
}

QFuture<void> LoginRegisterManager::getKey(QString path,
    std::unique_ptr<register_login::Login> login) {
  QString mes = "Generate key";
  float progress = 0.01f;
  emit updateLoadDbProgress(progress, mes);


  return QtConcurrent::run([this,path, login = std::move(login)]() {
    auto key = login->GetKey();
    QMetaObject::invokeMethod(this, [this, key]() {
            emit setNewKey(key);
        }, Qt::QueuedConnection);
    prepareForDecrypt(path).waitForFinished();
  });
}

QFuture<void> LoginRegisterManager::prepareForDecrypt(
    QString path) {
  return QtConcurrent::run([this, path]() mutable {
    float progress = 0.01f;

    auto userDb = std::make_unique<UserDb>(path);
    auto records = userDb->GetAllRecords();

    if (records.empty()) {
      return;
    }

    iterations_ = records[0].iterations;
    auto iterations = iterations_;

    size_t totalRecords = records.size();

    QVector<QFuture<RecordItem>> futures;

    for (auto i = 0; i < totalRecords; ++i) {
      auto record = records[i];
      QFuture<RecordItem> future = asyncDecrypt(record, totalRecords, progress);
      futures.append(future);
    }

    for (auto &future : futures) {
      future.waitForFinished();
      auto item = future.result();
      emit newItem(item);
    }
  });
}

QFuture<RecordItem> LoginRegisterManager::asyncDecrypt(
    EncryptedRecordItem eItem, size_t totalRecords, float progress) {
  return QtConcurrent::run([this, eItem, progress, totalRecords]()mutable ->RecordItem {
    std::vector<uint8_t> key;
    {
      QMutexLocker locker(&progressMutex_);
      key = key_;
    }
    auto item = EncryptDecrypt::DecryptRecordItem(eItem, key);
    {
      QMutexLocker locker(&progressMutex_);
      progress += 1.0f / totalRecords;
    }
    emit updateLoadDbProgress(progress, item.headlineText + " decrypted");
    return item;
  });
}

//================================= End namespace ==============================
}  // namespace rain_text::gui
