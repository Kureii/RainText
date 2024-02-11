/* This file was created by Kureii (Tomas Adamek)
 * Date created: 22. 01. 2024
 * This file is under the GPL-3 license
 */

#pragma once

#include <QColor>
#include <QJsonObject>
#include <QObject>
namespace rain_text::settings {

class SettingLoader : public QObject {
  Q_OBJECT
  Q_PROPERTY(bool hasError READ hasError NOTIFY errorChanged)
  Q_PROPERTY(QString errorHeadline READ errorHeadline NOTIFY errorChanged)
  Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY errorChanged)

 public:
  explicit SettingLoader(QObject *parent = nullptr);

  Q_INVOKABLE void ReloadSettings();

  Q_INVOKABLE void writeSettings(QString &name, QString &value);

  [[nodiscard]] bool hasError() const;
  [[nodiscard]] QString errorMessage() const;
  [[nodiscard]] QString errorHeadline() const;

  [[nodiscard]] QString GetCurrentColorMode() const;
  [[nodiscard]] QJsonObject GetColorModes() const;
  [[nodiscard]] QJsonObject GetColors() const;
  [[nodiscard]] int GetIterations() const;
 private:
  bool has_error_ = false;
  QString error_message_;
  QString error_headline_;
  QJsonObject json_data_;
  QString current_color_mode_;
  QJsonObject color_modes_;
  QJsonObject colors_;
  int iterations_;

  void LoadJsonFile();
  void LoadSettings();
  bool IsColorValid(const QString &colorString);
  void ValidateColors(const QJsonObject &colorModes);

 signals:
  void errorChanged();
  void errorMessage(QString &headline, QString &msg);
};

}  // namespace rain_text::settings