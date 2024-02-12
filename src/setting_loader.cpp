/* This file was created by Kureii (Tomas Adamek)
 * Date created: 22. 01. 2024
 * This file is under the GPL-3 license
 */
//================================= Includes ===================================
#include "RainText/setting_loader.hpp"

#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QRegularExpression>
#include <QCoreApplication>

//================================= Namespace ==================================
namespace rain_text::settings {

//================================= Public method ==============================
SettingLoader::SettingLoader(QObject *parent) : QObject(parent), translator_(new QTranslator) {
  LoadSettings();
}

SettingLoader::~SettingLoader() {
}

void SettingLoader::ReloadSettings() {
  QJsonObject uiSettings = json_data_["ui"].toObject();
  current_color_mode_ = uiSettings["colorMode"].toString();
  color_modes_ = uiSettings["colors"].toObject();
  colors_ = color_modes_[current_color_mode_].toObject();
  emit dataChanged();
}

void SettingLoader::ChangeCurrentColorMode(const QString &mode) {
  QJsonObject uiSettings = json_data_["ui"].toObject();
  uiSettings["colorMode"] = mode;
  current_color_mode_ = mode;
  json_data_["ui"] = uiSettings;

  ReloadSettings();

  QJsonDocument doc(json_data_);
  QByteArray json_bytes = doc.toJson();

  QFile file("settings.json");
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    qWarning() << "Nelze otevřít soubor pro zápis.";
    return;
  }

  file.write(json_bytes);
  file.close();
}

bool SettingLoader::hasError() const { return has_error_; }

QString SettingLoader::errorMessage() const { return error_message_; }

QString SettingLoader::errorHeadline() const { return error_headline_; }

QString SettingLoader::GetCurrentColorMode() const {
  return current_color_mode_;
}
int SettingLoader::GetIterations() const { return iterations_; }
QJsonObject SettingLoader::GetColorModes() const { return color_modes_; }
QJsonObject SettingLoader::appColors() const { return colors_; }
QStringList SettingLoader::GetValidThemes() const { return valid_themes_; }

QStringList SettingLoader::GetLangModel() const {
  QStringList model;
  auto languageFiles = GetLanguageFiles();
  for (auto &file: languageFiles) {
    model.push_front(file.split("-").first());
  }
  return model;
}

void SettingLoader::LoadLang() {
  if(translator_.load(":/locales/czech-cz_cs.qm")) {
    qApp->installTranslator(&translator_);
  }
}


//================================= Testing method =============================
#ifdef ENABLE_TESTS

#endif

//================================= Private method =============================
void SettingLoader::LoadJsonFile() {
  QFile file("settings.json");
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    error_headline_ = QObject::tr("Settings file");
    error_message_ = QObject::tr("The settings file cannot be opened.");
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
    error_headline_ = QObject::tr("Settings file");
    error_message_ = QObject::tr("The settings file contains an unexpected structure.");
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
      for (QString& required_color_key : expected_colors_) {
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

  std::reverse(validThemes.begin(), validThemes.end());
  valid_themes_ = validThemes;

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
      error_headline_ = QObject::tr("Settings file");
      error_message_ = QString(QObject::tr("Invalid color")+" %1: %2").arg(key, colorString);
      has_error_ = true;
      qWarning() << "------------------------\n"
                 << error_headline_ << "\n"
                 << error_message_ << "\n--------------------------------";
      return;
    }
  }
}

QStringList SettingLoader::GetLanguageFiles() const{
  QDir dir(":/locales");
  return dir.entryList(QStringList() << "*.qm", QDir::Files);
}

//================================= End namespace ==============================
}  // namespace rain_text::settings
