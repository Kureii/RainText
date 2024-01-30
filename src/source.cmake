set(gui_sources
        ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/login_register_manager.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/generate_password.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/copy_to_clipboard.cpp
)

set(setting_sources
        ${CMAKE_CURRENT_SOURCE_DIR}/src/settings/setting_loader.cpp
)

set(register_login_sources
        ${CMAKE_CURRENT_SOURCE_DIR}/src/register_login/register.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/register_login/login.cpp
)

set(hash_sources
        ${CMAKE_CURRENT_SOURCE_DIR}/src/hash/hash.cpp
)

set(sources
        ${gui_sources}
        ${setting_sources}
        ${register_login_sources}
        ${hash_sources}
)

set(executable_sources
        ${sources}
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)