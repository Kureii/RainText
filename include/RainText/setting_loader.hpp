/* This file was created by Kureii (Tomas Adamek)
 * Date created: 22. 01. 2024
 * This file is under the GPL-3 license
 */

#pragma once

#include <QColor>
#include <QJsonObject>
#include <QObject>
#include <QTranslator>

namespace rain_text::settings {

class SettingLoader : public QObject {
  Q_OBJECT
  Q_PROPERTY(bool hasError READ hasError NOTIFY errorChanged)
  Q_PROPERTY(QString errorHeadline READ errorHeadline NOTIFY errorChanged)
  Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY errorChanged)
  Q_PROPERTY(QJsonObject appColors READ appColors NOTIFY dataChanged)

 public:
  explicit SettingLoader(QObject *parent = nullptr);
  ~SettingLoader();

  Q_INVOKABLE void ReloadSettings();
  Q_INVOKABLE void ChangeCurrentColorMode(const QString &mode);

  [[nodiscard]] bool hasError() const;
  [[nodiscard]] QString errorMessage() const;
  [[nodiscard]] QString errorHeadline() const;

  Q_INVOKABLE [[nodiscard]] QString GetCurrentColorMode() const;
  Q_INVOKABLE [[nodiscard]] QJsonObject GetColorModes() const;
  Q_INVOKABLE [[nodiscard]] QJsonObject appColors() const;
  Q_INVOKABLE [[nodiscard]] int GetIterations() const;
  Q_INVOKABLE [[nodiscard]] QStringList GetValidThemes() const;

  Q_INVOKABLE [[nodiscard]] QStringList GetLangModel() const;
  Q_INVOKABLE void LoadLang();
  Q_INVOKABLE [[nodiscard]] QString GetCurrentLangName() const;
 private:
  bool has_error_ = false;
  QTranslator translator_;
  QString error_message_;
  QString error_headline_;
  QJsonObject json_data_;
  QString current_color_mode_;
  QJsonObject color_modes_;
  QJsonObject colors_;
  int iterations_;
  QStringList valid_themes_;
  QStringList expected_colors_ = {"primaryColor",
                                 "primaryPressedColor",
                                 "primaryHoveredColor",
                                 "notEnablePrimaryColor",
                                 "textColor",
                                 "textColorOnPrimary",
                                 "notEnableTextColor",
                                 "focusColor",
                                 "textOnPrimaryColor",
                                 "focusOnPrimaryColor",
                                 "backgroundColor",
                                 "drawerBackgroundColor",
                                 "borderButtonColor",
                                 "borderButtonFocusColor",
                                 "notEnabledBorderButtonColor",
                                 "borderButtonHoveredColor",
                                 "borderButtonPressedColor",
                                 "registerLoginSwapButtonBorderColor",
                                 "registerLoginSwapButtonFillColor",
                                 "registerLoginSwapButtonFillHoveredColor",
                                 "registerLoginSwapButtonFillPressedColor",
                                 "loadProgressBarBackground",
                                 "loginRegisterBackgroundColor"};

  void LoadJsonFile();
  void LoadSettings();
  bool IsColorValid(const QString &colorString);
  void ValidateColors(const QJsonObject &colorModes);
  QStringList GetLanguageFiles() const;

 signals:
  void errorChanged();
  void errorMessage(QString &headline, QString &msg);
  void dataChanged();
};

}  // namespace rain_text::settings