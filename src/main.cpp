/*
#define MYCODE 1

#if MYCODE == 1
#include "headers/gui.h"

int main(int argc, char *argv[]) {
  auto gui = new rain_text::Gui("com.example.login_and_password_manager");
  auto err = gui->Run();
  delete gui;
  return err;
}

#elif MYCODE == 2

#include <sqlite3.h>
#include <iostream>


static int callback(void *data, int argc, char **argv, char **azColName) {
  for (int i = 0; i < argc; i++) {
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0;
}

int main() {
  sqlite3 *db;
  char *err_msg = nullptr;
  int rc;

  rc = sqlite3_open("test.db", &db);

  if (rc != SQLITE_OK) {
    fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return 1;
  }

  const char *sql =
      "CREATE TABLE IF NOT EXISTS People(Id INT, Name TEXT);"
      "INSERT INTO People (Id, Name) VALUES (1, 'Alice');"
      "INSERT INTO People (Id, Name) VALUES (2, 'Bob');"
      "SELECT * FROM People;";

  rc = sqlite3_exec(db, sql, callback, nullptr, &err_msg);

  if (rc != SQLITE_OK) {
    fprintf(stderr, "SQL error: %s\n", err_msg);
    sqlite3_free(err_msg);
    sqlite3_close(db);
    return 1;
  }

  sqlite3_close(db);

  return 0;
}

#elif MYCODE == 3

#include <iostream>
#include <map>
#include <string>

int main() {
  std::map<std::string, int> ages;

  ages["Alice"] = 30;
  ages["Bob"] = 25;
  ages["Charlie"] = 22;

  std::string key = "Alice";
  int new_value = 35;

  auto it = ages.find(key);
  if (it != ages.end()) {
    // Klíč existuje, změnit hodnotu
    it->second = new_value;
  } else {
    // Klíč neexistuje, vytvořit nový klíč s danou hodnotou
    ages[key] = new_value;
  }

  // Výpis mapy
  for (const auto& entry : ages) {
    std::cout << entry.first << " is " << entry.second << " years old." << std::endl;
  }

  return 0;
}

#elif  MYCODE == 4

#include "headers/user_database.h"

int main() {
  auto data = rain_text::UserDatabase("./users/testdb.db");
  return 0;
}

#elif MYCODE == 5

#include <iostream>
#include <string>
#include <algorithm>
#include <cstdint>

int main() {
  uint8_t separators[] = {0x1C, 0x1D, 0x1E, 0x1F};
  std::string input = "Hello\x1EWorld";

  auto separator = std::find_first_of(input.begin(), input.end(), std::begin(separators), std::end(separators));

  std::string first(input.begin(), separator);
  std::string second(separator + 1, input.end());

  std::cout << "First:-" << first << "-" << std::endl;
  std::cout << "Second:-" << second << "-" << std::endl;

  return 0;
}

#elif MYCODE == 6

#include <iostream>
#include <vector>

int main() {
  std::vector<int> numbers{1, 2, 3, 4, 5};
  size_t position = 2; // N-tá pozice, kterou chcete smazat (indexováno od 0)

  if (position < numbers.size()) {
    numbers.erase(numbers.begin() + position);
  } else {
    std::cout << "Invalid position." << std::endl;
  }

  // Výpis upraveného vektoru
  for (int number : numbers) {
    std::cout << number << " ";
  }

  return 0;
}


#endif*/

#include <iostream>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQuick/QQuickView>
#include <QtQml/qqml.h>
#include <QQmlContext>
#include "RainText/gui/login_register_manager.h"
#include "RainText/gui/generate_password.h"

int main(int argc, char* argv[]) {
  QGuiApplication app(argc, argv);

  QQmlApplicationEngine engine;
  rain_text::gui::LoginRegisterManager login_register_manager;
  rain_text::gui::GeneratePassword generate_password;
  engine.rootContext()->setContextProperty("login_register_manager", &login_register_manager);
  engine.rootContext()->setContextProperty("generate_password", &generate_password);

  const QUrl url(QStringLiteral("qrc:/main.qml"));
  QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
      &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
          QCoreApplication::exit(-1);
      }, Qt::QueuedConnection);
  engine.load(url);

  return QGuiApplication::exec();
}