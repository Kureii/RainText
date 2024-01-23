set(raintext_gui_headers
        ${CMAKE_CURRENT_SOURCE_DIR}/include/RainText/gui/login_register_manager.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/RainText/gui/generate_password.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/RainText/gui/copy_to_clipboard.hpp
)

set(raintext_settings_headers
        ${CMAKE_CURRENT_SOURCE_DIR}/include/RainText/settings/setting_loader.hpp
)

set(all_headers
    ${raintext_gui_headers}
    ${raintext_settings_headers}
)