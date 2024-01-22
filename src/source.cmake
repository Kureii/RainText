set(gui_sources
        ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/login_register_manager.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/generate_password.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/copy_to_clipboard.cpp
)

set(setting_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/src/settings/setting_loader.cpp
)

set(sources
    ${gui_sources}
    ${setting_sources}
)

set(executable_sources
        ${sources}
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)