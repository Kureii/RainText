/* This file was created by Kureii (Tomas Adamek)
 * Date created: 19. 11. 2023
 * This file is under the GPL-3 license
 */

#pragma once

#include <QObject>
#include <QFutureWatcher>
#include "RainText/record_list_model.hpp"
#include "RainText/structs.hpp"

namespace rain_text::gui {

class LoginRegisterManager : public QObject {
  Q_OBJECT
  Q_PROPERTY(model::RecordListModel* recordListModel READ recordListModel NOTIFY recordListModelChanged)
 public:
  explicit LoginRegisterManager(QObject *parent = nullptr);

  Q_INVOKABLE void ConfirmFormUser(const QString &username,
                                   const QString &password);

  Q_INVOKABLE void StateChange(const QString &new_state);

  Q_INVOKABLE bool CheckFields(const QString &username, const QString &password,
                               const QString &password_again);

  Q_INVOKABLE float PasswordStrength(const QString &password);

  model::RecordListModel* recordListModel() const {
    return m_recordListModel_;
  }
 private:
  std::string state_;
  QFutureWatcher<void> watcher_;
  model::RecordListModel *m_recordListModel_;

 signals:
  void recordListModelChanged();
  void registrationComplete();
  void loginComplete();
  void errorOccurred(const QString &error);
  void loadDb(const QString &title);
  void updateLoadDbProgress(float progress, const QString &message);
  void newItem(RecordItem item);

  public slots:
    void onAsyncOperationFinished();
    void handleNewItem(RecordItem item);
};

}  // namespace rain_text::gui
