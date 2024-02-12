cmake_minimum_required(VERSION 3.26)

project(RainText VERSION 0.5.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_definitions(-DMAIN_DB_PATH=\"./database/main.db\")
add_definitions(-DMAX_THREADS=8)

find_package(Qt6 COMPONENTS
        Core
        Quick
        Qml
        Gui
        Widgets
        Svg
        Sql
        Concurrent
        REQUIRED
)

include(./src/source.cmake)
include(./include/include.cmake)
include(./qml/qrc.cmake)
add_subdirectory(lib)

qt_add_executable(${PROJECT_NAME}
        ${qrc}
        ${executable_sources}
        ${all_headers}
        ${CMAKE_CURRENT_SOURCE_DIR}/locales/locales.qrc
)

target_include_directories(${PROJECT_NAME}
        PUBLIC ./include/
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib/MD5
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib/SHA3IUF/
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib/argon2/include
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib/libscrypt
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib/RainTextCore/include
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/json/settings.json
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)


target_link_libraries(${PROJECT_NAME}
        PRIVATE Qt::Core
        PRIVATE Qt::Quick
        PRIVATE Qt::Qml
        PRIVATE Qt::Gui
        PRIVATE Qt::Widgets
        PRIVATE Qt::Svg
        PRIVATE Qt::Sql
        PRIVATE Qt6::Concurrent
        md5_lib
        sha3lib::sha3
        scryptlib::scrypt
        argon2lib::argon2
        RainTextCore
)