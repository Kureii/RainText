set(raintext_gui_headers
        ${CMAKE_CURRENT_SOURCE_DIR}/include/RainText/gui/login_register_manager.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/RainText/gui/generate_password.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/RainText/gui/copy_to_clipboard.hpp
)

set(raintext_another_headers
        ${CMAKE_CURRENT_SOURCE_DIR}/include/RainText/setting_loader.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/RainText/register.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/RainText/login.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/RainText/hash.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/RainText/record_list_model.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/RainText/user_db.hpp
)


set(all_headers
        ${raintext_gui_headers}
        ${raintext_another_headers}
)