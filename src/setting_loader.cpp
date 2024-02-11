/* This file was created by Kureii (Tomas Adamek)
 * Date created: 22. 01. 2024
 * This file is under the GPL-3 license
 */
//================================= Includes ===================================
#include "RainText/setting_loader.hpp"

#include <QFile>
#include <QJsonDocument>
#include <QRegularExpression>

//================================= Namespace ==================================
namespace rain_text::settings {

//================================= Public method ==============================
SettingLoader::SettingLoader(QObject *parent) : QObject(parent) {
  LoadSettings();
}
void SettingLoader::ReloadSettings() {}
void SettingLoader::writeSettings(QString &name, QString &value) {}

bool SettingLoader::hasError() const { return has_error_; }

QString SettingLoader::errorMessage() const { return error_message_; }

QString SettingLoader::errorHeadline() const { return error_headline_; }

QString SettingLoader::GetCurrentColorMode() const {
  return current_color_mode_;
}
int SettingLoader::GetIterations() const { return iterations_; }
QJsonObject SettingLoader::GetColorModes() const { return color_modes_; }
QJsonObject SettingLoader::GetColors() const { return colors_; }

//================================= Testing method =============================
#ifdef ENABLE_TESTS

#endif

//================================= Private method =============================
void SettingLoader::LoadJsonFile() {
  QFile file("settings.json");
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    error_headline_ = "Settings file";
    error_message_ = "The settings file cannot be opened.";
    has_error_ = true;
    qWarning() << "------------------------\n"
               << error_headline_ << "\n"
               << error_message_ << "\n--------------------------------";
    return;
  }

  QString json_string = file.readAll();
  file.close();

  QJsonDocument json_document = QJsonDocument::fromJson(json_string.toUtf8());
  if (json_document.isNull()) {
    error_headline_ = "Settings file";
    error_message_ = "The settings file contains an unexpected structure.";
    has_error_ = true;
    qWarning() << "------------------------\n"
               << error_headline_ << "\n"
               << error_message_ << "\n--------------------------------";
    return;
  }

  json_data_ = json_document.object();
}

void SettingLoader::LoadSettings() {
  LoadJsonFile();
  iterations_ = json_data_["encryptIterations"].toInt();
  QStringList expected_colors = {"primaryColor",
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

  QJsonObject uiSettings = json_data_["ui"].toObject();
  current_color_mode_ = uiSettings["colorMode"].toString();
  color_modes_ = uiSettings["colors"].toObject();
  colors_ = color_modes_[current_color_mode_].toObject();

  QStringList validThemes;
  for (const QString& themeName : color_modes_.keys()) {
    if (themeName.contains("dark") || themeName.contains("light")) {
      QJsonObject themeColorsObj = color_modes_[themeName].toObject();
      auto themeColorsKeys = themeColorsObj.keys();
      bool isValid = true;
      for (QString& required_color_key : expected_colors) {
        if (!themeColorsKeys.contains(required_color_key)) {
          isValid = false;
          qDebug() << themeName << "is missing color key:" << required_color_key;
          break;
        }
      }
      if (isValid) {
        validThemes.append(themeName);
      }
    } else {
      qDebug() << themeName << "does not contain 'dark' or 'light'.";
    }
  }


  ValidateColors(colors_);
}

bool SettingLoader::IsColorValid(const QString &colorString) {
  QRegularExpression regex(
      "^#(?:[0-9a-fA-F]{3}){1,2}$|^#(?:[0-9a-fA-F]{4}){1,2}$");
  return regex.match(colorString).hasMatch();
}

void SettingLoader::ValidateColors(const QJsonObject &colors) {
  foreach (const QString &key, colors.keys()) {
    QString colorString = colors[key].toString();
    if (!IsColorValid(colorString)) {
      error_headline_ = "Settings file";
      error_message_ = QString("Invalid colour %1: %2").arg(key, colorString);
      has_error_ = true;
      qWarning() << "------------------------\n"
                 << error_headline_ << "\n"
                 << error_message_ << "\n--------------------------------";
      return;
    }
  }
}

//================================= End namespace ==============================
}  // namespace rain_text::settings
