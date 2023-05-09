#include "gtest/gtest.h"
#include "../src/headers/user_database.h"

static rain_text::Record records[] = {
    {1, "Bank", "111111111111", "BankPassword"},
    {2, "Email", "info@info.cz", "EmailPassword"},
    {3, "GitHub", "Kureii", "GitHubPassword"}
};

TEST(UserDatabase_IdManager, AddItem) {
  auto input = std::vector<uint8_t>({1,1});
  auto output = std::vector<uint8_t>({1,1,1});
  auto testdb = new rain_text::UserDatabase("./test/id_manager_add_item.db");
  auto plain_text = std::vector<rain_text::Record>({records[0], records[1]});
  testdb->SetIdManager(input);
  testdb->SetPlainData(plain_text);
  testdb->GetId();
  auto result = testdb->GetIdManager();
  delete testdb;
  ASSERT_EQ(result, output);
}

TEST(UserDatabase_IdManager, RemoveEndItem) {
  auto input = std::vector<uint8_t>({1,1,1});
  auto output = std::vector<uint8_t>({1,1});
  auto testdb = new rain_text::UserDatabase("./test/id_manager_remove_last.db");
  auto plain_text = std::vector<rain_text::Record>({records[0], records[1], records[2]});
  testdb->SetIdManager(input);
  for (auto & i: plain_text) {
    testdb->SetData(i);
  }
  testdb->DeleteData(3);
  auto result = testdb->GetIdManager();
  delete testdb;
  ASSERT_EQ(result, output);
}

TEST(UserDatabase_IdManager, RemoveMiddleItem) {
  auto input = std::vector<uint8_t>({1,1,1});
  auto output = std::vector<uint8_t>({1,0,1});
  auto testdb = new rain_text::UserDatabase("./test/id_manager_remove_middle.db");
  auto plain_text = std::vector<rain_text::Record>({records[0], records[1], records[2]});
  testdb->SetIdManager(input);
  for (auto & i: plain_text) {
    testdb->SetData(i);
  }
  testdb->DeleteData(2);
  auto result = testdb->GetIdManager();
  delete testdb;
  ASSERT_EQ(result, output);
}

TEST(UserDatabase_IdManager, FillZero) {
  auto input = std::vector<uint8_t>({1, 0, 1});
  auto output = std::vector<uint8_t>({1, 1, 1});
  auto testdb = new rain_text::UserDatabase("./test/id_manager_fill_zero.db");
  auto plain_text = std::vector<rain_text::Record>({records[0], records[2]});
  testdb->SetIdManager(input);
  testdb->SetPlainData(plain_text);
  testdb->GetId();
  auto result = testdb->GetIdManager();
  delete testdb;
  ASSERT_EQ(result, output);
}

TEST(UserDatabase_IdManager, RemoveLastOneItem) {
  auto input = std::vector<uint8_t>({0,0,1});
  auto testdb = new rain_text::UserDatabase("./test/id_manager_remove_lastone_item.db");
  testdb->SetData(records[2]);
  testdb->SetIdManager(input);
  testdb->DeleteData(3);
  auto result = testdb->GetIdManager();
  delete testdb;
  ASSERT_TRUE(result.empty());
}

TEST(UserDatabase_PrepareData, Separator0x1C) {
  std::string pre_input[] = {"Kureii", "3", "GitHub\x1CGitHubPassword"};
  int size = sizeof(pre_input) / sizeof(pre_input[0]);
  std::vector<const char*> cstring_input(size);
  for (int i = 0; i < size; ++i) {
    cstring_input[i] = pre_input[i].c_str();
  }
  char** input = const_cast<char**>(cstring_input.data());

  rain_text::Record output = records[2];
  auto testdb = new rain_text::UserDatabase("./test/prepare_data_0x1c.db");
  auto result = testdb->PrepareDataTest(input);
  delete testdb;
  ASSERT_EQ(result, output);
}