//================================= Includes ===================================
#include "headers/gui.h"

#include <iostream>
#include <map>
#include <thread>

#include "headers/pass_gen.h"

//================================= Namespace ==================================
namespace rain_text {

//======================== Define helpful variables ============================

//======================= Define helpful structures ============================

//======================= Define helpful functions =============================

//============================= Gui implementation =============================
GtkApplication* Gui::app_ = nullptr;
GtkWidget* Gui::login_window_ = nullptr;
GtkWidget* Gui::main_window_ = nullptr;
UserDatabase* Gui::u_db_ = nullptr;
MainDatabase* Gui::m_db_ = nullptr;

Gui::Gui(const char* app_id) {
#ifdef _WIN32
  app_ = gtk_application_new(app_id, G_APPLICATION_DEFAULT_FLAGS);
#else
  app_ = gtk_application_new(app_id, G_APPLICATION_FLAGS_NONE);
#endif
    g_signal_connect(app_, "activate", G_CALLBACK(OnActivate), this);
}

Gui::~Gui() {
  delete u_db_;
  g_object_unref(app_);
}

int Gui::Run() { return g_application_run(G_APPLICATION(app_), 0, nullptr); }

void Gui::OnActivate(GtkApplication* app, gpointer user_data) {
  Gui* self = static_cast<Gui*>(user_data);
  self->LoginWindow(app);
}

//------------------------------- App windows ----------------------------------
void Gui::LoginWindow(GtkApplication* app) {
  login_window_ = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(login_window_), "RainText Login");
  gtk_window_set_default_size(GTK_WINDOW(login_window_), 800, 600);

  auto stack = gtk_stack_new();

  //------------------------------- Login gui ----------------------------------
  auto login_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_widget_set_halign(login_box, GTK_ALIGN_CENTER);
  gtk_widget_set_valign(login_box, GTK_ALIGN_CENTER);
  gtk_widget_set_margin_top(login_box, 10);
  gtk_widget_set_margin_bottom(login_box, 12);
  gtk_widget_set_margin_start(login_box, 8);
  gtk_widget_set_margin_end(login_box, 8);
  gtk_widget_set_size_request(login_box, 250, 200);

  auto login_headline = gtk_label_new("Přihlášení");

  auto login_separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);

  auto login_username_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_widget_set_halign(login_username_box, GTK_ALIGN_FILL);
  gtk_widget_set_valign(login_username_box, GTK_ALIGN_CENTER);
  gtk_widget_set_size_request(login_username_box, 250, 30);
  auto login_username_label = gtk_label_new("Uživatelské jméno: ");
  auto login_username_entry = gtk_entry_new();
  gtk_widget_set_halign(login_username_label, GTK_ALIGN_START);
  gtk_widget_set_halign(login_username_entry, GTK_ALIGN_END);

  gtk_box_append(GTK_BOX(login_username_box), login_username_label);
  gtk_box_append(GTK_BOX(login_username_box), login_username_entry);

  auto login_password_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  gtk_widget_set_halign(login_password_box, GTK_ALIGN_FILL);
  gtk_widget_set_valign(login_password_box, GTK_ALIGN_CENTER);
  gtk_widget_set_size_request(login_password_box, 250, 30);

  auto login_password_label = gtk_label_new("Heslo: ");
  gtk_widget_set_halign(login_password_label, GTK_ALIGN_START);
  auto login_password_entry = gtk_password_entry_new();
  gtk_widget_set_halign(login_password_entry, GTK_ALIGN_END);
  gtk_password_entry_set_show_peek_icon(GTK_PASSWORD_ENTRY(login_password_entry),
                                        TRUE);

  gtk_box_append(GTK_BOX(login_password_box), login_password_label);
  gtk_box_append(GTK_BOX(login_password_box), login_password_entry);

  g_object_set_data(G_OBJECT(login_window_), "login_username_entry", login_username_entry);
  g_object_set_data(G_OBJECT(login_window_), "login_password_entry", login_password_entry);
  g_signal_connect(login_username_entry, "changed", G_CALLBACK(AreLoginEntriesFilled),
                   nullptr);
  g_signal_connect(login_password_entry, "changed", G_CALLBACK(AreLoginEntriesFilled),
                   nullptr);

  auto login_login_btn = gtk_button_new_with_label("Přihlásit se");
  gtk_widget_set_sensitive(login_login_btn, FALSE);
  gtk_widget_set_size_request(login_login_btn, 250, 30);

  auto login_separator2 = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);

  auto login_register_btn = gtk_button_new_with_label("Registrovat se");
  gtk_widget_set_size_request(login_login_btn, 250, 30);

  g_object_set_data(G_OBJECT(login_window_), "login_login_btn", login_login_btn);
  // TODO: metod for login
  // g_signal_connect(login_btn, "clicked", G_CALLBACK(AddItem),
  // &add_item_window);
  g_object_set_data(G_OBJECT(login_window_), "login_register_btn", login_register_btn);
  g_signal_connect(login_register_btn, "clicked", G_CALLBACK(SwitchLoginRegister),
                   stack);

  gtk_box_append(GTK_BOX(login_box), login_headline);
  gtk_box_append(GTK_BOX(login_box), login_separator);
  gtk_box_append(GTK_BOX(login_box), login_username_box);
  gtk_box_append(GTK_BOX(login_box), login_password_box);
  gtk_box_append(GTK_BOX(login_box), login_separator2);
  gtk_box_append(GTK_BOX(login_box), login_login_btn);
  gtk_box_append(GTK_BOX(login_box), login_register_btn);

  //----------------------------- Register gui ---------------------------------
  auto register_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_widget_set_halign(register_box, GTK_ALIGN_CENTER);
  gtk_widget_set_valign(register_box, GTK_ALIGN_CENTER);
  gtk_widget_set_margin_top(register_box, 10);
  gtk_widget_set_margin_bottom(register_box, 12);
  gtk_widget_set_margin_start(register_box, 8);
  gtk_widget_set_margin_end(register_box, 8);
  gtk_widget_set_size_request(register_box, 250, 200);

  auto register_headline = gtk_label_new("Registrace");

  auto register_separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);

  auto register_username_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_widget_set_halign(register_username_box, GTK_ALIGN_FILL);
  gtk_widget_set_valign(register_username_box, GTK_ALIGN_CENTER);
  gtk_widget_set_size_request(register_username_box, 250, 30);
  auto register_username_label = gtk_label_new("Uživatelské jméno: ");
  auto register_username_entry = gtk_entry_new();
  gtk_widget_set_halign(register_username_label, GTK_ALIGN_START);
  gtk_widget_set_halign(register_username_entry, GTK_ALIGN_END);

  gtk_box_append(GTK_BOX(register_username_box), register_username_label);
  gtk_box_append(GTK_BOX(register_username_box), register_username_entry);

  auto register_password_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  gtk_widget_set_halign(register_password_box, GTK_ALIGN_FILL);
  gtk_widget_set_valign(register_password_box, GTK_ALIGN_CENTER);
  gtk_widget_set_size_request(register_password_box, 250, 30);

  auto register_password_label = gtk_label_new("Heslo: ");
  gtk_widget_set_halign(register_password_label, GTK_ALIGN_START);
  auto register_password_entry = gtk_password_entry_new();
  gtk_widget_set_halign(register_password_entry, GTK_ALIGN_END);
  gtk_password_entry_set_show_peek_icon(GTK_PASSWORD_ENTRY(register_password_entry),
                                        TRUE);
  gtk_box_append(GTK_BOX(register_password_box), register_password_label);
  gtk_box_append(GTK_BOX(register_password_box), register_password_entry);

  auto register_password_again_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  gtk_widget_set_halign(register_password_again_box, GTK_ALIGN_FILL);
  gtk_widget_set_valign(register_password_again_box, GTK_ALIGN_CENTER);
  gtk_widget_set_size_request(register_password_again_box, 250, 30);

  auto register_password_again_label = gtk_label_new("Heslo znovu: ");
  gtk_widget_set_halign(register_password_again_label, GTK_ALIGN_START);
  auto register_password_again_entry = gtk_password_entry_new();
  gtk_widget_set_halign(register_password_again_entry, GTK_ALIGN_END);
  gtk_password_entry_set_show_peek_icon(GTK_PASSWORD_ENTRY(register_password_again_entry),
                                        TRUE);

  gtk_box_append(GTK_BOX(register_password_again_box), register_password_again_label);
  gtk_box_append(GTK_BOX(register_password_again_box), register_password_again_entry);

  g_object_set_data(G_OBJECT(login_window_), "register_username_entry", register_username_entry);
  g_object_set_data(G_OBJECT(login_window_), "register_password_entry", register_password_entry);
  g_object_set_data(G_OBJECT(login_window_), "register_password_again_entry", register_password_again_entry);
  g_signal_connect(register_username_entry, "changed", G_CALLBACK(AreRegisterEntriesFilled),
                   nullptr);
  g_signal_connect(register_password_entry, "changed", G_CALLBACK(AreRegisterEntriesFilled),
                   nullptr);
  g_signal_connect(register_password_again_entry, "changed", G_CALLBACK(AreRegisterEntriesFilled),
                   nullptr);

  auto register_login_btn = gtk_button_new_with_label("Přihlásit se");
  gtk_widget_set_size_request(register_login_btn, 250, 30);

  auto register_separator2 = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);

  auto register_register_btn = gtk_button_new_with_label("Registrovat se");
  gtk_widget_set_sensitive(register_register_btn, FALSE);
  gtk_widget_set_size_request(register_register_btn, 250, 30);

  g_object_set_data(G_OBJECT(login_window_), "register_register_btn", register_register_btn);
  g_signal_connect(register_register_btn, "clicked", G_CALLBACK(RegisterUser),
                   nullptr);
  g_object_set_data(G_OBJECT(login_window_), "register_login_btn", register_login_btn);
  g_signal_connect(register_login_btn, "clicked", G_CALLBACK(SwitchLoginRegister),
  stack);

  gtk_box_append(GTK_BOX(register_box), register_headline);
  gtk_box_append(GTK_BOX(register_box), register_separator);
  gtk_box_append(GTK_BOX(register_box), register_username_box);
  gtk_box_append(GTK_BOX(register_box), register_password_box);
  gtk_box_append(GTK_BOX(register_box), register_password_again_box);
  gtk_box_append(GTK_BOX(register_box), register_separator2);
  gtk_box_append(GTK_BOX(register_box), register_register_btn);
  gtk_box_append(GTK_BOX(register_box), register_login_btn);

  gtk_stack_add_named(GTK_STACK(stack), login_box, "Login");
  gtk_stack_add_named(GTK_STACK(stack), register_box, "Register");

  gtk_stack_set_visible_child_name(GTK_STACK(stack), "Login");

  gtk_window_set_child(GTK_WINDOW(login_window_), stack);

  gtk_window_present(GTK_WINDOW(login_window_));
}


void Gui::MainWindow(GtkApplication* app) {
  // Create and configure the main window
  main_window_ = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(main_window_), "RainText");
  gtk_window_set_default_size(GTK_WINDOW(main_window_), 800, 600);

  //--------------------------- GUI widgets ------------------------------------
  // Create and configure the GUI widgets for the main window
  auto top_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_widget_set_valign(top_box, GTK_ALIGN_FILL);
  gtk_widget_set_halign(top_box, GTK_ALIGN_FILL);
  auto scrolled = gtk_scrolled_window_new();
  gtk_scrolled_window_set_min_content_height(GTK_SCROLLED_WINDOW(scrolled),
                                             200);
  gtk_scrolled_window_set_has_frame(GTK_SCROLLED_WINDOW(scrolled), TRUE);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled),
                                 GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
  gtk_scrolled_window_set_propagate_natural_height(
      GTK_SCROLLED_WINDOW(scrolled), TRUE);
  gtk_widget_set_vexpand(scrolled, TRUE);

  auto box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
  gtk_widget_set_valign(box, GTK_ALIGN_FILL);
  gtk_widget_set_margin_top(box, 10);
  gtk_widget_set_margin_bottom(box, 12);
  gtk_widget_set_margin_start(box, 8);
  gtk_widget_set_margin_end(box, 8);
  gtk_widget_set_size_request(box, 718, -1);

  auto content = u_db_->GetData();

  // Load the user data and create the list of password items
  if (content.empty()) {
    GtkWidget* info_label =
        gtk_label_new("Pro přidání záznamu klikněte na tlačítko +");
    gtk_widget_set_halign(info_label, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(info_label, GTK_ALIGN_CENTER);
    gtk_widget_set_size_request(info_label, -1, -1);
    gtk_box_append(GTK_BOX(box), info_label);
  } else {
    for (size_t i = 0; i < content.size(); i++) {
      auto record = content[i];
      GtkWidget* list_item = CreateListItem(record);
      gtk_box_append(GTK_BOX(box), list_item);
      if (i < content.size() - 1) {
        gtk_box_append(GTK_BOX(box),
                       gtk_separator_new(GTK_ORIENTATION_HORIZONTAL));
      }
    }
  }

  // Append the widgets to the parent containers
  gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled), box);

  g_object_set_data(G_OBJECT(main_window_), "password_list", box);

  // Create and configure the Add button
  auto add_btn = gtk_button_new();
  gtk_widget_set_size_request(add_btn, 50, 50);
  gtk_widget_set_halign(add_btn, GTK_ALIGN_END);
  gtk_widget_set_valign(add_btn, GTK_ALIGN_END);
  gtk_widget_set_margin_top(add_btn, 16);
  gtk_widget_set_margin_bottom(add_btn, 16);
  gtk_widget_set_margin_start(add_btn, 16);
  gtk_widget_set_margin_end(add_btn, 16);

  auto add_icon = gtk_image_new_from_file("./../assets/icons/iconsax/add.svg");
  // Append the widgets to the parent containers
  gtk_button_set_child(GTK_BUTTON(add_btn), add_icon);

  // Connect the signals to the appropriate callbacks
  g_signal_connect(add_btn, "clicked", G_CALLBACK(ShowAddItemModal), nullptr);

  // Append the widgets to the parent containers
  gtk_box_append(GTK_BOX(top_box), scrolled);
  gtk_box_append(GTK_BOX(top_box), add_btn);

  gtk_window_set_child(GTK_WINDOW(main_window_), top_box);

  // Present the main window to the user
  gtk_window_present(GTK_WINDOW(main_window_));
}

void Gui::ShowAddItemModal(GtkButton* button, gpointer user_data) {
  // Create and configure the "Add Item" window
  auto add_item_window = gtk_window_new();
  gtk_window_set_title(GTK_WINDOW(add_item_window), "Add Item");
  gtk_window_set_default_size(GTK_WINDOW(add_item_window), 300, 300);

  gtk_window_set_modal(GTK_WINDOW(add_item_window), TRUE);
  // gtk_window_set_transient_for(GTK_WINDOW(add_item_window),
  //                              GTK_WINDOW(static_cast<GtkWidget*>(user_data)));
  g_object_set_data(G_OBJECT(main_window_), "add_item_modal", add_item_window);

  //--------------------------- GUI widgets ------------------------------------
  // Create and configure the GUI widgets for the "Add Item" window
  auto box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
  gtk_widget_set_valign(box, GTK_ALIGN_CENTER);
  gtk_widget_set_margin_top(box, 10);
  gtk_widget_set_margin_bottom(box, 12);
  gtk_widget_set_margin_start(box, 8);
  gtk_widget_set_margin_end(box, 8);
  gtk_widget_set_size_request(box, 250, 200);

  auto headline_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  auto headline_label = gtk_label_new("Nadpis: ");
  auto headline_entry = gtk_entry_new();
  gtk_widget_set_halign(headline_entry, GTK_ALIGN_END);

  // Append the widgets to the parent containers
  gtk_box_append(GTK_BOX(headline_box), headline_label);
  gtk_box_append(GTK_BOX(headline_box), headline_entry);

  auto separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);

  auto username_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_widget_set_halign(username_box, GTK_ALIGN_FILL);
  gtk_widget_set_valign(username_box, GTK_ALIGN_CENTER);
  gtk_widget_set_size_request(username_box, 250, 30);
  auto username_label = gtk_label_new("Uživatelské jméno: ");
  auto username_entry = gtk_entry_new();
  gtk_widget_set_halign(username_label, GTK_ALIGN_START);
  gtk_widget_set_halign(username_entry, GTK_ALIGN_END);

  // Append the widgets to the parent containers
  gtk_box_append(GTK_BOX(username_box), username_label);
  gtk_box_append(GTK_BOX(username_box), username_entry);

  auto password_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_widget_set_halign(password_box, GTK_ALIGN_FILL);
  gtk_widget_set_valign(password_box, GTK_ALIGN_CENTER);
  gtk_widget_set_size_request(password_box, 250, 60);

  auto password_entry_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_widget_set_size_request(password_entry_box, 250, 30);
  gtk_widget_set_halign(password_entry_box, GTK_ALIGN_FILL);

  auto password_label = gtk_label_new("Heslo: ");
  gtk_widget_set_halign(password_label, GTK_ALIGN_START);
  auto password_entry = gtk_password_entry_new();
  gtk_widget_set_halign(password_entry, GTK_ALIGN_END);
  gtk_password_entry_set_show_peek_icon(GTK_PASSWORD_ENTRY(password_entry),
                                        TRUE);

  // Append the widgets to the parent containers
  gtk_box_append(GTK_BOX(password_entry_box), password_label);
  gtk_box_append(GTK_BOX(password_entry_box), password_entry);

  auto generate_btn = gtk_button_new_with_label("genervat heslo");
  gtk_widget_set_size_request(generate_btn, 250, 25);

  // Append the widgets to the parent containers
  gtk_box_append(GTK_BOX(password_box), password_entry_box);
  gtk_box_append(GTK_BOX(password_box), generate_btn);

  auto separator2 = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);

  auto save_btn = gtk_button_new_with_label("Uložit");
  gtk_widget_set_sensitive(save_btn, FALSE);
  gtk_widget_set_size_request(save_btn, 250, 30);

  g_object_set_data(G_OBJECT(main_window_), "save_btn", save_btn);
  g_object_set_data(G_OBJECT(main_window_), "headline_entry", headline_entry);
  g_object_set_data(G_OBJECT(main_window_), "username_entry", username_entry);
  g_object_set_data(G_OBJECT(main_window_), "password_entry", password_entry);
  // Connect the signals to the appropriate callbacks
  g_signal_connect(generate_btn, "clicked", G_CALLBACK(OnGenerateBtnClicked),
                   password_entry);
  g_signal_connect(headline_entry, "changed", G_CALLBACK(AreAddEntriesFilled),
                   nullptr);
  g_signal_connect(username_entry, "changed", G_CALLBACK(AreAddEntriesFilled),
                   nullptr);
  g_signal_connect(password_entry, "changed", G_CALLBACK(AreAddEntriesFilled),
                   nullptr);
  g_signal_connect(save_btn, "clicked", G_CALLBACK(AddItem), &add_item_window);

  // Append the widgets to the parent containers
  gtk_box_append(GTK_BOX(box), headline_box);
  gtk_box_append(GTK_BOX(box), separator);
  gtk_box_append(GTK_BOX(box), username_box);
  gtk_box_append(GTK_BOX(box), password_box);
  gtk_box_append(GTK_BOX(box), separator2);
  gtk_box_append(GTK_BOX(box), save_btn);

  gtk_window_set_child(GTK_WINDOW(add_item_window), box);

  // Present the "Add Item" window to the user
  gtk_window_present(GTK_WINDOW(add_item_window));
}

//----------------------------- Entry changed ----------------------------------
void Gui::AreAddEntriesFilled(GtkEditable* object, gpointer user_data) {
  GtkWidget* headline_entry =
      GTK_WIDGET(g_object_get_data(G_OBJECT(main_window_), "headline_entry"));
  GtkWidget* username_entry =
      GTK_WIDGET(g_object_get_data(G_OBJECT(main_window_), "username_entry"));
  GtkWidget* password_entry =
      GTK_WIDGET(g_object_get_data(G_OBJECT(main_window_), "password_entry"));
  GtkWidget* save_btn =
      GTK_WIDGET(g_object_get_data(G_OBJECT(main_window_), "save_btn"));

  auto headline =
      std::string(gtk_editable_get_text(GTK_EDITABLE(headline_entry)));
  auto username =
      std::string(gtk_editable_get_text(GTK_EDITABLE(username_entry)));
  auto password =
      std::string(gtk_editable_get_text(GTK_EDITABLE(password_entry)));

  if (headline.empty() || username.empty() || password.empty()) {
    gtk_widget_set_sensitive(save_btn, FALSE);
  } else {
    gtk_widget_set_sensitive(save_btn, TRUE);
  }
}

void Gui::AreLoginEntriesFilled(GtkEditable* object, gpointer user_data) {
  GtkWidget* username_entry =
      GTK_WIDGET(g_object_get_data(G_OBJECT(login_window_), "login_username_entry"));
  GtkWidget* password_entry =
      GTK_WIDGET(g_object_get_data(G_OBJECT(login_window_), "login_password_entry"));
  GtkWidget* save_btn =
      GTK_WIDGET(g_object_get_data(G_OBJECT(login_window_), "login_login_btn"));

  auto username =
      std::string(gtk_editable_get_text(GTK_EDITABLE(username_entry)));
  auto password =
      std::string(gtk_editable_get_text(GTK_EDITABLE(password_entry)));

  if (username.empty() || password.empty()) {
    gtk_widget_set_sensitive(save_btn, FALSE);
  } else {
    gtk_widget_set_sensitive(save_btn, TRUE);
  }
}

void Gui::AreRegisterEntriesFilled(GtkEditable *object, gpointer data) {
  GtkWidget* username_entry =
      GTK_WIDGET(g_object_get_data(G_OBJECT(login_window_), "register_username_entry"));
  GtkWidget* password_entry =
      GTK_WIDGET(g_object_get_data(G_OBJECT(login_window_), "register_password_entry"));
  GtkWidget* password_again_entry =
      GTK_WIDGET(g_object_get_data(G_OBJECT(login_window_), "register_password_again_entry"));
  GtkWidget* save_btn =
      GTK_WIDGET(g_object_get_data(G_OBJECT(login_window_), "register_register_btn"));

  auto username =
      std::string(gtk_editable_get_text(GTK_EDITABLE(username_entry)));
  auto password =
      std::string(gtk_editable_get_text(GTK_EDITABLE(password_entry)));
  auto password_again =
      std::string(gtk_editable_get_text(GTK_EDITABLE(password_again_entry)));

  /* TODO: interactive password
   * if (!password.empty() && password_again.empty()) {
   *   if(password == password_again) {
   *
   *    }
   * }
   */

  if (username.empty() || password.size() < 7 || password_again.size() < 7 || password_again != password) {
    gtk_widget_set_sensitive(save_btn, FALSE);
  } else {
    gtk_widget_set_sensitive(save_btn, TRUE);
  }
}

//----------------------------- Button clicked ---------------------------------
void Gui::AddItem(GtkWidget* object, gpointer user_data) {
  GtkWidget* password_list =
      GTK_WIDGET(g_object_get_data(G_OBJECT(main_window_), "password_list"));
  GtkWidget* headline_entry =
      GTK_WIDGET(g_object_get_data(G_OBJECT(main_window_), "headline_entry"));
  GtkWidget* username_entry =
      GTK_WIDGET(g_object_get_data(G_OBJECT(main_window_), "username_entry"));
  GtkWidget* password_entry =
      GTK_WIDGET(g_object_get_data(G_OBJECT(main_window_), "password_entry"));
  GtkWidget* add_item_modal =
      GTK_WIDGET(g_object_get_data(G_OBJECT(main_window_), "add_item_modal"));

  auto headline =
      std::string(gtk_editable_get_text(GTK_EDITABLE(headline_entry)));
  auto username =
      std::string(gtk_editable_get_text(GTK_EDITABLE(username_entry)));
  auto password =
      std::string(gtk_editable_get_text(GTK_EDITABLE(password_entry)));
  size_t id = u_db_->GetId();
  Record record = {id, headline, username, password};
  Record non_move_record = record;
  std::thread thread(&UserDatabase::SetData, u_db_, std::move(record));

  auto children = gtk_widget_observe_children(password_list);
  uint32_t num_children = g_list_model_get_n_items(children);

  if (num_children == 1) {
    auto child = (GtkWidget*)g_list_model_get_item(children, 0);
    if (GTK_IS_LABEL(child)) {
      gtk_widget_unparent(child);
    } else {
      gtk_box_append(GTK_BOX(password_list),
                     gtk_separator_new(GTK_ORIENTATION_HORIZONTAL));
    }
  } else {
    gtk_box_append(GTK_BOX(password_list),
                   gtk_separator_new(GTK_ORIENTATION_HORIZONTAL));
  }

  gtk_box_append(GTK_BOX(password_list), CreateListItem(non_move_record));
  gtk_window_destroy(GTK_WINDOW(add_item_modal));
  thread.join();
}

void Gui::OnGenerateBtnClicked(GtkButton* button, gpointer user_data) {
  //TODO: Password options
  auto password_entry = GTK_PASSWORD_ENTRY(user_data);
  std::string password = pass_gen::GeneratePassword(16);

  gtk_editable_set_text(GTK_EDITABLE(password_entry), password.c_str());
  auto entry_password =
      std::string(gtk_editable_get_text(GTK_EDITABLE(password_entry)));
  if (entry_password != password) {
    OnGenerateBtnClicked(button, user_data);
  }
}

void Gui::DeleteParent(GtkButton* button, gpointer user_data) {
  GtkWidget* parent =
      gtk_widget_get_parent(gtk_widget_get_parent(GTK_WIDGET(button)));

  if (parent) {
    auto id_string = gtk_widget_get_name(parent);
    std::cout << id_string << std::endl;
    auto id = std::stoi(id_string);

    std::thread thread(&UserDatabase::DeleteData, u_db_, id);

    GtkWidget* next_sibling = gtk_widget_get_next_sibling(parent);
    GtkWidget* parent_of_parent = gtk_widget_get_parent(parent);
    GtkWidget* prev_sibling = gtk_widget_get_prev_sibling(parent);
    if (!next_sibling && prev_sibling) {
      gtk_widget_unparent(prev_sibling);
    }
    if (next_sibling) {
      gtk_widget_unparent(next_sibling);
    }

    next_sibling = gtk_widget_get_next_sibling(parent);

    if (!next_sibling && !prev_sibling) {
      GtkWidget* info_label =
          gtk_label_new("Pro přidání záznamu klikněte na tlačítko +");
      gtk_widget_set_halign(info_label, GTK_ALIGN_CENTER);
      gtk_widget_set_valign(info_label, GTK_ALIGN_CENTER);
      gtk_widget_set_size_request(info_label, -1, -1);

      if (GTK_IS_BOX(parent_of_parent)) {
        gtk_box_append(GTK_BOX(parent_of_parent), info_label);
      } else if (GTK_IS_GRID(parent_of_parent)) {
        gtk_grid_attach(GTK_GRID(parent_of_parent), info_label, 0, 0, 1, 1);
      } else {
        g_warning("Unsupported parent widget for adding label info.");
      }
    }
    gtk_widget_unparent(parent);
    thread.join();
  }
}

void Gui::ShowHidePasswd(GtkButton* button, gpointer user_data) {
  auto row = GTK_BOX(user_data);
  auto headline = gtk_widget_get_first_child(GTK_WIDGET(row));
  auto passwd = gtk_widget_get_next_sibling(headline);
  auto btn_img = gtk_widget_get_first_child(
      GTK_WIDGET(gtk_widget_get_next_sibling(passwd)));
  auto img_paths = std::string(gtk_widget_get_name(btn_img));

  char delimiter = '\x1C';
  std::vector<std::string> tokens;

  std::size_t pos = 0;
  while ((pos = img_paths.find(delimiter)) != std::string::npos) {
    tokens.push_back(img_paths.substr(0, pos));
    img_paths.erase(0, pos + 1);
  }
  tokens.push_back(img_paths);

  gtk_image_set_from_file(GTK_IMAGE(btn_img), tokens[0].c_str());
  auto icon_paths = tokens[1] + "\x1C" + tokens[0];

  gtk_widget_set_name(btn_img, icon_paths.c_str());

  auto new_passwd = gtk_widget_get_name(passwd);
  auto old_passwd = gtk_label_get_label(GTK_LABEL(passwd));
  gtk_label_set_selectable(GTK_LABEL(passwd),
                           !gtk_label_get_selectable(GTK_LABEL(passwd)));
  gtk_widget_set_name(passwd, old_passwd);
  gtk_label_set_label(GTK_LABEL(passwd), new_passwd);
}

void Gui::SwitchLoginRegister(GtkButton* button, gpointer user_data) {
  GtkStack *stack = GTK_STACK(user_data);
  const gchar *current_child = gtk_stack_get_visible_child_name(stack);

  if (g_strcmp0(current_child, "Login") == 0) {
    gtk_stack_set_visible_child_name(stack, "Register");
  } else {
    gtk_stack_set_visible_child_name(stack, "Login");
  }
}

void Gui::RegisterUser(GtkButton* button, gpointer user_data) {
  GtkWidget* username_entry =
      GTK_WIDGET(g_object_get_data(G_OBJECT(login_window_), "register_username_entry"));
  GtkWidget* password_entry =
      GTK_WIDGET(g_object_get_data(G_OBJECT(login_window_), "register_password_entry"));
  std::list<std::string> list;
  list.push_back(std::string(gtk_editable_get_text(GTK_EDITABLE(username_entry))));
  list.push_back(std::string(gtk_editable_get_text(GTK_EDITABLE(password_entry))));
  auto m_db = new MainDatabase("./database/central.db");
  m_db->CreateUser(list);
  delete m_db;
}
//---------------------------- Create widgets ----------------------------------
GtkWidget* Gui::CreateListItem(Record& data) {
  auto item = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  auto id = std::to_string(data.content_id);
  gtk_widget_set_name(item, id.c_str());

  gtk_widget_set_size_request(item, 300, 60);
  auto record = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  auto headline = gtk_label_new(data.headline.c_str());
  gtk_widget_set_halign(headline, GTK_ALIGN_START);
  gtk_label_set_selectable(GTK_LABEL(headline), TRUE);

  auto username_row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  auto username_lable = gtk_label_new("Uživatelské jméno: ");
  auto username_lable2 = gtk_label_new(data.username.c_str());
  gtk_label_set_selectable(GTK_LABEL(username_lable2), TRUE);

  gtk_box_append(GTK_BOX(username_row), username_lable);
  gtk_box_append(GTK_BOX(username_row), username_lable2);

  auto password_row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  auto password_lable = gtk_label_new("Heslo: ");
  auto password_lable2 = gtk_label_new("•••••••••••••");
  gtk_widget_set_name(password_lable2, data.password.c_str());

  auto password_btn = gtk_button_new();
  auto password_btn_icon =
      gtk_image_new_from_file("./../assets/icons/iconsax/eye-slash.svg");
  gtk_widget_set_name(password_btn_icon,
                      "./../assets/icons/iconsax/eye.svg\x1C./../assets/icons/"
                      "iconsax/eye-slash.svg");
  gtk_button_set_child(GTK_BUTTON(password_btn), password_btn_icon);

  g_signal_connect(password_btn, "clicked", G_CALLBACK(ShowHidePasswd),
                   password_row);

  gtk_box_append(GTK_BOX(password_row), password_lable);
  gtk_box_append(GTK_BOX(password_row), password_lable2);
  gtk_box_append(GTK_BOX(password_row), password_btn);

  gtk_box_append(GTK_BOX(record), headline);
  gtk_box_append(GTK_BOX(record), username_row);
  gtk_box_append(GTK_BOX(record), password_row);

  auto edit_icon_btn = gtk_button_new();
  gtk_widget_set_size_request(edit_icon_btn, 50, -1);
  auto edit_icon =
      gtk_image_new_from_file("./../assets/icons/iconsax/edit-2.svg");
  gtk_button_set_child(GTK_BUTTON(edit_icon_btn), edit_icon);

  auto separator_v = gtk_separator_new(GTK_ORIENTATION_VERTICAL);

  auto del_icon_btn = gtk_button_new();
  gtk_widget_set_size_request(edit_icon_btn, 30, -1);
  auto del_icon =
      gtk_image_new_from_file("./../assets/icons/iconsax/trash.svg");
  gtk_button_set_child(GTK_BUTTON(del_icon_btn), del_icon);

  auto options = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_widget_set_halign(options, GTK_ALIGN_END);
  gtk_box_append(GTK_BOX(options), edit_icon_btn);
  gtk_box_append(GTK_BOX(options), separator_v);
  gtk_box_append(GTK_BOX(options), del_icon_btn);

  gtk_box_append(GTK_BOX(item), record);
  gtk_box_append(GTK_BOX(item), options);

  g_signal_connect(del_icon_btn, "clicked", G_CALLBACK(DeleteParent), NULL);
#ifdef DEBUG
  std::cout << "-------------------------------" << std::endl;
  std::cout << "headline: " << data.headline << std::endl;
  std::cout << "username: " << data.username << std::endl;
  std::cout << "password: " << data.password << std::endl;
  std::cout << "content_id: " << data.content_id << std::endl;
  std::cout << "-------------------------------" << std::endl << std::endl;
#endif
  return item;
}
//===================== Implement helpful functions ============================


}  // namespace rain_text
