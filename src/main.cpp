

#include <iostream>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQuick/QQuickView>
#include <QQmlContext>
#include "RainText/gui/login_register_manager.hpp"
#include "RainText/gui/generate_password.hpp"
#include "RainText/gui/copy_to_clipboard.hpp"
#include "RainText/settings/setting_loader.hpp"

int main(int argc, char* argv[]) {
  QGuiApplication app(argc, argv);

  QQmlApplicationEngine engine;
  rain_text::gui::LoginRegisterManager login_register_manager;
  rain_text::gui::GeneratePassword generate_password;
  rain_text::gui::CopyToClipboard copy_to_clipboard;
  rain_text::settings::SettingLoader settings_manager;

  engine.rootContext()->setContextProperty("login_register_manager", &login_register_manager);
  engine.rootContext()->setContextProperty("generate_password", &generate_password);
  engine.rootContext()->setContextProperty("copy_to_clipboard", &copy_to_clipboard);
  engine.rootContext()->setContextProperty("settings_manager", &settings_manager);
  engine.rootContext()->setContextProperty("appColors", settings_manager.GetColors());
  engine.rootContext()->setContextProperty("appColorModes", settings_manager.GetColorModes());
  engine.rootContext()->setContextProperty("appCurrentColorMode", settings_manager.GetCurrentColorMode());

  const QUrl url(QStringLiteral("qrc:/main.qml"));
  QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
      &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
          QCoreApplication::exit(-1);
      }, Qt::QueuedConnection);
  engine.load(url);

  return QGuiApplication::exec();
}