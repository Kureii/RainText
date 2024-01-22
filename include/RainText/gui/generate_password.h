/* This file was created by Kureii (Tomas Adamek)
 * Date created: 19. 01. 2024
 * This file is under the GPL-3 license
 */

#pragma once

#include <QObject>
namespace rain_text::gui {

class GeneratePassword : public QObject {
  Q_OBJECT
 public:
  explicit GeneratePassword(QObject *parent = nullptr);

  Q_INVOKABLE void ClickToGenerateButton(bool majusculeGenPass,
                                         bool minusculeGenPass,
                                         bool numberGenPass,
                                         bool lspecialGenPass, int length);

 signals:
  void passwordGenerated(const QString &password);
};

}  // namespace rain_text::gui