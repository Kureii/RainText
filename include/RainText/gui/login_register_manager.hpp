/* This file was created by Kureii (Tomas Adamek)
 * Date created: 19. 11. 2023
 * This file is under the GPL-3 license
 */

#pragma once

#include <QObject>
#include <QFutureWatcher>
#include "RainText/record_list_model.hpp"
#include "RainText/structs.hpp"
#include "RainText/enrollment_manager.hpp"
#include "RainText/login.hpp"
#include "RainText/setting_loader.hpp"

namespace rain_text::gui {

class LoginRegisterManager : public QObject {
Q_OBJECT
Q_PROPERTY(model::RecordListModel* recordListModel READ recordListModel NOTIFY recordListModelChanged)
Q_PROPERTY(int iterations READ iterations NOTIFY iterationsChanged)
public:
  explicit LoginRegisterManager(QObject *parent = nullptr);

  void SetIterations(int iterations);

  Q_INVOKABLE void ConfirmFormUser(const QString &username,
                                   const QString &password);

  Q_INVOKABLE void StateChange(const QString &new_state);

  Q_INVOKABLE bool CheckFields(const QString &username, const QString &password,
                               const QString &password_again);

  Q_INVOKABLE float PasswordStrength(const QString &password);

  Q_INVOKABLE void IterationsChanged(float iterations);
  model::RecordListModel* recordListModel() const;

  int iterations() const;
private:
  std::string state_;
  QFutureWatcher<void> watcher_;
  QMutex progressMutex_;
  model::RecordListModel *m_recordListModel_;
  EnrollmentManager* pEnrollmentManager_;
  std::vector<uint8_t> key_;
  QThread *enrollmentManagerThread;
  EnrollmentManager *enrollmentManager;
  int iterations_;
  QString path_;

  void startEnrollmentThread();
  QFuture<void> getKey(QString path, std::unique_ptr<register_login::Login> login);
  QFuture<void> prepareForDecrypt(QString path);
  QFuture<RecordItem> asyncDecrypt(EncryptedRecordItem eItem, size_t totalRecords, float progress);

signals:
  void recordListModelChanged();
  void iterationsChanged();
  void registrationComplete();
  void loginComplete();
  void errorOccurred(const QString &error);
  void loadDb(const QString &title);
  void updateLoadDbProgress(float progress, const QString &message);
  void newItem(RecordItem item);
  void setNewKey(std::vector<uint8_t> key);
  void terminateThread(const QList<RecordItem> &listModel, int iterations);
  void loginError(const QString &msg);
  void registerError(const QString &msg);

public slots:
  void onAsyncOperationFinished();
  void handleNewItem(RecordItem item);
  void onSetKey(std::vector<uint8_t> key);
  void onAppQuit();
};

}  // namespace rain_text::gui
