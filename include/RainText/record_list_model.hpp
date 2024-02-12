/* This file was created by Kureii (Tomas Adamek)
 * Date created: 07. 02. 2024
 * This file is under the GPL-3 license
 */

#pragma once

#include <QAbstractListModel>
#include <QList>
#include "RainText/structs.hpp"

namespace rain_text::model {



class RecordListModel : public QAbstractListModel {
  Q_OBJECT
public:
  enum RecordRoles {
    HeadlineRole = Qt::UserRole + 1,
    UsernameRole,
    PasswordRole
  };

  explicit RecordListModel(QObject *parent = nullptr);

  Q_INVOKABLE int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;

  void addRecordItem(const RecordItem &item);
  Q_INVOKABLE void addRecordItem(const QString& headline, const QString& username, const QString& password, const int iterations);
  Q_INVOKABLE void removeRecordItem(int index, const int iterations);
  Q_INVOKABLE void moveRecordItem(int fromIndex, int toIndex, const int iterations);
  Q_INVOKABLE void editRecordItem(int index, const QString& headline, const QString& username, const QString& password, const int iterations);

  void IterationsChanged(int iterations);

  [[nodiscard]] QList<RecordItem> GetList();
private:
  QList<RecordItem> m_items;

signals:
  void saveChanges(const QList<RecordItem> &listModel, int iterations);
  void savingComplete();

public slots:
  void savingFinished();
};

}

