/* This file was created by Kureii (Tomas Adamek)
 * Date created: 07. 02. 2024
 * This file is under the GPL-3 license
 */

#pragma once

#include <QAbstractListModel>
#include <QList>

namespace rain_text::model {

struct RecordItem {
  QString headlineText;
  QString usernameText;
  QString passwordText;
};

class RecordListModel : public QAbstractListModel {
  Q_OBJECT
public:
  enum RecordRoles {
    HeadlineRole = Qt::UserRole + 1,
    UsernameRole,
    PasswordRole
  };

  explicit RecordListModel(QObject *parent = nullptr);

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;

  void addRecordItem(const RecordItem &item);
  Q_INVOKABLE void addRecordItem(const QString& headline, const QString& username, const QString& password);
  Q_INVOKABLE void removeRecordItem(int index);
  Q_INVOKABLE void moveRecordItem(int fromIndex, int toIndex);
  Q_INVOKABLE void editRecordItem(int index, const QString& headline, const QString& username, const QString& password);

private:
  QList<RecordItem> m_items;
};

}

