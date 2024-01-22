/* This file was created by Kureii (Tomas Adamek)
 * Date created: 22. 01. 2024
 * This file is under the GPL-3 license
 */

#pragma once

#include <QObject>
namespace rain_text::settings {

class SettingLoader : public QObject {
  Q_OBJECT
public:
  explicit SettingLoader(QObject *parent = nullptr);


};


}