/* This file was created by Kureii (Tomas Adamek)
 * Date created: 07. 02. 2023
 * This file is under the GPL-3 license
 */
//================================= Includes ===================================

#include "RainText/record_list_model.hpp"

//================================= Namespace ==================================
namespace rain_text::model {

//================================= Public method ==============================
RecordListModel::RecordListModel(QObject *parent)
    : QAbstractListModel(parent) {
}

int RecordListModel::rowCount(const QModelIndex &parent) const {
  Q_UNUSED(parent);
  return m_items.count();
}

QVariant RecordListModel::data(const QModelIndex &index, int role) const {
  if (index.row() < 0 || index.row() >= m_items.count())
    return QVariant();

  const RecordItem &item = m_items[index.row()];
  switch (role) {
    case HeadlineRole: return item.headlineText;
    case UsernameRole: return item.usernameText;
    case PasswordRole: return item.passwordText;
    default: return QVariant();
  }
}

QHash<int, QByteArray> RecordListModel::roleNames() const {
  QHash<int, QByteArray> roles;
  roles[HeadlineRole] = "headlineText";
  roles[UsernameRole] = "usernameText";
  roles[PasswordRole] = "passwordText";
  return roles;
}

void RecordListModel::addRecordItem(const RecordItem &item) {
  beginInsertRows(QModelIndex(), rowCount(), rowCount());
  m_items << item;
  endInsertRows();
}

void RecordListModel::addRecordItem(const QString& headline, const QString& username, const QString& password) {
  auto item = RecordItem{headline, username, password};
  addRecordItem(item);
}

void RecordListModel::removeRecordItem(int index) {
  if (index < 0 || index >= m_items.size())
    return;

  beginRemoveRows(QModelIndex(), index, index);
  m_items.removeAt(index);
  endRemoveRows();
}

void RecordListModel::moveRecordItem(int fromIndex, int toIndex) {
  if (fromIndex < 0 || fromIndex >= m_items.size() || toIndex < 0 || toIndex >= m_items.size() || fromIndex == toIndex)
    return;

  beginMoveRows(QModelIndex(), fromIndex, fromIndex, QModelIndex(), toIndex > fromIndex ? toIndex + 1 : toIndex);
  m_items.move(fromIndex, toIndex);
  endMoveRows();
}

void RecordListModel::editRecordItem(int index, const QString& headline, const QString& username, const QString& password) {
  if (index < 0 || index >= m_items.size())
    return;

  RecordItem& item = m_items[index];
  item.headlineText = headline;
  item.usernameText = username;
  item.passwordText = password;

  emit dataChanged(this->index(index), this->index(index), {HeadlineRole, UsernameRole, PasswordRole});
}
//================================= Testing method =============================
#ifdef ENABLE_TESTS

#endif

//================================= Private method =============================

//================================= End namespace ==============================
}  // namespace rain_text::model
