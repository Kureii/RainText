/* This file was created by Kureii (Tomas Adamek)
* Date created: 21. 01. 2024
 * This file is under the GPL-3 license
 */

#pragma once

#include <QObject>
namespace rain_text::gui {

class CopyToClipboard : public QObject {
  Q_OBJECT

public:
  explicit CopyToClipboard(QObject *parent = nullptr);

  Q_INVOKABLE void copyToClipboard(const QString &text);
};
}
