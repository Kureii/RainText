set(sources
        ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/login_register_manager.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/generate_password.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/copy_to_clipboard.cpp
)

set(executable_sources
        ${sources}
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)