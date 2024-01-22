set(raintext_gui_headers
        ${CMAKE_CURRENT_SOURCE_DIR}/include/RainText/gui/login_register_manager.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/RainText/gui/generate_password.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/RainText/gui/copy_to_clipboard.h
)

set(raintext_settings_headers
        ${CMAKE_CURRENT_SOURCE_DIR}/include/RainText/settings/setting_loader.h
)

set(all_headers
    ${raintext_gui_headers}
    ${raintext_settings_headers}
)