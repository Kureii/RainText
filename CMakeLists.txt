cmake_minimum_required(VERSION 3.22)
set(PROJECT_NAME RainText)

project(${PROJECT_NAME} VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

if (CMAKE_SYSTEM_NAME  STREQUAL "Linux")
    # Linux
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK4 REQUIRED IMPORTED_TARGET gtk4)
elseif (CMAKE_SYSTEM_NAME  STREQUAL "Windows")
    # Windows
    set(CMAKE_EXE_LINKER_FLAGS "-Wl,-Bdynamic")
    # Nastavení cesty k GTK4 instalaci
    set(GTK4_INSTALL_DIR "C:/msys64/mingw64" CACHE PATH "GTK4 installation directory")

    # Nastavení proměnných pro knihovny a záhlaví
    set(GTK4_INCLUDE_DIRS "${GTK4_INSTALL_DIR}/include/gtk-4.0;${GTK4_INSTALL_DIR}/include/glib-2.0;${GTK4_INSTALL_DIR}/lib/glib-2.0/include;${GTK4_INSTALL_DIR}/include/pango-1.0;${GTK4_INSTALL_DIR}/include/harfbuzz;${GTK4_INSTALL_DIR}/include/cairo;${GTK4_INSTALL_DIR}/include/gdk-pixbuf-2.0;${GTK4_INSTALL_DIR}/include/gio-win32-2.0;${GTK4_INSTALL_DIR}/include/atk-1.0;${GTK4_INSTALL_DIR}/include/graphene-1.0;${GTK4_INSTALL_DIR}/lib/graphene-1.0/include;${GTK4_INSTALL_DIR}/lib/glibmm-2.68/include")
    set(GTK4_LIBRARY_DIRS "${GTK4_INSTALL_DIR}/lib")

    # Nastavení CMake pro nalezení knihovny GTK4
    find_library(GTK4_LIBRARIES gtk-4 HINTS ${GTK4_LIBRARY_DIRS})
elseif (CMAKE_SYSTEM_NAME  STREQUAL "Darwin")
    # macOS
    message(FATAL_ERROR "Not supported yet")
else ()
    message(FATAL_ERROR "Not supported")
endif ()

find_package(SQLite3 REQUIRED)
find_package(Threads REQUIRED)

add_subdirectory(lib)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/gui.cpp
        src/hash.cpp
        src/pass_gen.cpp
        src/user_database.cpp
        src/main_database.cpp
        src/database_utils.cpp
        src/headers/gui.h
        src/headers/hash.h
        src/headers/pass_gen.h
        src/headers/user_database.h
        src/headers/main_database.h
        src/headers/database_utils.h
        )


if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    # Linux
    target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/rain_text_core/include
            #[[${CMAKE_CURRENT_SOURCE_DIR}/lib/MD5
            ${CMAKE_CURRENT_SOURCE_DIR}/lib/SHA3IUF/
            ${CMAKE_CURRENT_SOURCE_DIR}/lib/phc-winner-argon2/include
            ${CMAKE_CURRENT_SOURCE_DIR}/lib/libscrypt]]
    )
    target_link_libraries(${PROJECT_NAME}
            PkgConfig::GTK4
            SQLite::SQLite3
            Threads::Threads
            md5_lib
            sha3lib::sha3
            scryptlib::scrypt
            argon2lib::argon2
            RainTextCore
            )
elseif (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    # Windows
    target_include_directories(${PROJECT_NAME} PRIVATE
            ${GTK4_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/lib/rain_text_core/include
            #[[${CMAKE_CURRENT_SOURCE_DIR}/lib/MD5
            ${CMAKE_CURRENT_SOURCE_DIR}/lib/SHA3IUF
            ${CMAKE_CURRENT_SOURCE_DIR}/lib/phc-winner-argon2/include
            ${CMAKE_CURRENT_SOURCE_DIR}/lib/libscrypt]]
            )
    target_link_directories(RainText PRIVATE
            ${GTK4_LIBRARY_DIRS}
            )
    target_link_libraries(${PROJECT_NAME} PRIVATE
            ${GTK4_LIBRARIES}
            gobject-2.0
            gio-2.0
            glib-2.0
            gdk_pixbuf-2.0
            SQLite::SQLite3
            Threads::Threads
            md5_lib
            sha3lib::sha3
            scryptlib::scrypt
            argon2lib::argon2
            RainTextCore
            -static-libgcc -static-libstdc++)
    # Získání cesty k potřebným knihovnám
    list(GET GTK4_LIBRARY_DIRS 0 GTK4_LIB_DIR)

    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

    file(GLOB DLL_FILES "${GTK4_LIB_DIR}/*.dll")
    file(COPY ${DLL_FILES} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})




elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    # macOS
    message(FATAL_ERROR "Not supported yet")
else ()
    message(FATAL_ERROR "Not supported yet")
endif ()

target_compile_options(RainText PRIVATE ${GTK4_CFLAGS_OTHER})

enable_testing()

add_subdirectory(tests)
