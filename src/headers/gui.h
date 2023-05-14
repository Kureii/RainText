#ifndef RAINTEXT_GUI_H
#define RAINTEXT_GUI_H

#include <gtk/gtk.h>

#include "main_database.h"
#include "user_database.h"

namespace rain_text {

class Gui {
 public:
  explicit Gui(const char* app_id);
  ~Gui();
  int Run();
  static GtkApplication* app_;
  static GtkWidget* main_window_;
  static UserDatabase* u_db_;
  static MainDatabase* m_db_;

 private:
  static std::vector<uint8_t> key_;
  static std::vector<EncryptRecord> cyphered_data_;
  static std::vector<Record> plain_data_;
  /**
   * @brief Determines if the add item entries are filled and adjusts the save
   * button sensitivity.
   *
   * This function is designed to be called as a callback for a GtkEditable
   * widget. It checks if the headline, username, and password entry fields are
   * filled, and adjusts the sensitivity of the save button accordingly.
   *
   * @param object The GtkEditable instance that triggered the callback.
   * @param user_data A gpointer, but not used in this function.
   */
  static void AreAddEntriesFilled(GtkEditable* object, gpointer data);

  /**
   * @brief Determines if the login entries are filled and adjusts the save
   * button sensitivity.
   *
   * This function is designed to be called as a callback for a GtkEditable
   * widget. It checks if the username and password entry fields are filled, and
   * adjusts the sensitivity of the save button accordingly.
   *
   * @param object The GtkEditable instance that triggered the callback.
   * @param user_data A gpointer, but not used in this function.
   */
  static void AreLoginEntriesFilled(GtkEditable* object, gpointer data);

  static void AreRegisterEntriesFilled(GtkEditable* object, gpointer data);

  /**
   * @brief Creates and shows the "Add Item" modal window.
   *
   * This function is designed to be called as a callback for a GtkButton
   * widget. It creates a new "Add Item" modal window, which includes fields for
   * entering headline, username, and password, as well as buttons for
   * generating a password and saving the new item.
   *
   * @param button The GtkButton instance that triggered the callback.
   * @param user_data A gpointer, but not used in this function.
   */
  static void ShowAddItemModal(GtkButton* button, gpointer user_data);

  /**
   * @brief Adds a new item to the password list and saves it to the
   * UserDatabase.
   *
   * This function is designed to be called as a callback for a GtkButton or
   * another GtkWidget. It retrieves the input values from the entry fields,
   * creates a new Record object, and adds it to the UserDatabase. The function
   * also updates the password list UI with the newly created item.
   *
   * @param object The GtkWidget instance that triggered the callback.
   * @param user_data A gpointer, but not used in this function.
   */
  static void AddItem(GtkWidget* object, gpointer user_data);

  /**
   * @brief Handles the "activate" signal for the GtkApplication.
   *
   * This function is called when the application is activated. It is
   * responsible for showing the login window of the RainText application.
   *
   * @param app The GtkApplication instance that has been activated.
   * @param user_data A pointer to the Gui instance handling the signal.
   */
  static void OnActivate(GtkApplication* app, gpointer user_data);

  void LoginWindow(GtkApplication* app);

  /**
   * @brief Creates and shows the main window of the RainText application.
   *
   * This function sets up the main window, including the layout and widgets,
   * and connects the appropriate signals to their corresponding callback
   * functions.
   */
  static void MainWindow();

  /**
   * @brief Generates a random password and sets it in the password entry.
   *
   * This function is designed to be called as a callback for a GtkButton. When
   * invoked, it generates a random password of length 16 using the pass_gen
   * library and sets the text of the GtkPasswordEntry to the generated
   * password.
   *
   * @param button The GtkButton instance that triggered the callback.
   * @param user_data A gpointer pointing to the GtkPasswordEntry widget where
   * the generated password should be set.
   */
  static void OnGenerateBtnClicked(GtkButton* button, gpointer user_data);

  /**
   * @brief Creates a list item widget for displaying a record.
   *
   * This function takes a Record as input and creates a GtkWidget containing
   * the record's data. The resulting widget includes labels for the headline,
   * username, and a masked password, as well as buttons for editing and
   * deleting the record.
   *
   * @param data The Record object containing the data to display in the list
   * item.
   * @return GtkWidget* A pointer to the newly created list item widget.
   */
  static GtkWidget* CreateListItem(Record& data);

  /**
   * @brief Deletes the parent widget and associated data from the database.
   *
   * This function is designed to be called as a callback for a GtkButton. When
   * invoked, it deletes the parent widget of the button, removes the associated
   * data from the UserDatabase, and updates the UI accordingly.
   *
   * @param button The GtkButton instance that triggered the callback.
   * @param user_data A gpointer, but not used in this function.
   */
  static void DeleteParent(GtkButton* button, gpointer user_data);

  /**
   * @brief Toggles the visibility of a password and updates the button image.
   *
   * This function is designed to be called as a callback for a GtkButton. When
   * invoked, it shows or hides the password text, toggles the button image
   * between "show" and "hide" states, and updates the widget names accordingly.
   *
   * @param button The GtkButton instance that triggered the callback.
   * @param user_data A gpointer pointing to the GTK_BOX containing the password
   * and button widgets.
   */
  static void ShowHidePasswd(GtkButton* button, gpointer user_data);

  /**
   * @brief Switches between the login and registration pages in the user
   * interface.
   *
   * This function toggles the visibility of the login and registration pages in
   * the user interface using a GtkStack. When the button is clicked, the
   * function checks the currently visible child and switches to the other
   * child.
   *
   * @param button A pointer to the GtkButton that triggered the function.
   * @param user_data A gpointer holding the reference to the GtkStack
   * containing the login and registration pages.
   */
  static void SwitchLoginRegister(GtkButton* button, gpointer user_data);

  static void RegisterUser(GtkButton* button, gpointer user_data);

  static void LoginUser(GtkButton* button, gpointer user_data);
};
}  // namespace rain_text
#endif  // RAINTEXT_GUI_H
