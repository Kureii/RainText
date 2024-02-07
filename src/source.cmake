set(gui_sources
        ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/login_register_manager.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/generate_password.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/copy_to_clipboard.cpp
)

set(another_sources
        ${CMAKE_CURRENT_SOURCE_DIR}/src/setting_loader.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/hash.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/register.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/login.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/record_list_model.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/user_db.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/encrypt_decrypt.cpp


)

set(sources
        ${gui_sources}
        ${another_sources}
)

set(executable_sources
        ${sources}
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)