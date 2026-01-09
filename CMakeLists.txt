cmake_minimum_required(VERSION 3.16)

project(QtCards VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# Указываем где искать .ui файлы
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/src/ui)

# Включаем тесты по опции
option(BUILD_TESTS "Build tests" ON)

# Подключаем все поддиректории
# add_subdirectory(src)

if(BUILD_TESTS)
    add_subdirectory(src/tests) # Подключит src/tests/CMakeLists.txt
endif()

# Собираем файлы
file(GLOB_RECURSE SOURCES "src/cpp/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h")
file(GLOB_RECURSE FORMS "src/ui/*.ui")

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(QtCards
        MANUAL_FINALIZATION
        ${SOURCES}
        ${HEADERS}
        ${FORMS}
    )
else()
    add_executable(QtCards
        ${SOURCES}
        ${HEADERS}
        ${FORMS}
    )
endif()

# Указываем где искать заголовочные файлы
target_include_directories(QtCards PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include  # для #include "mainwindow.h"
    ${CMAKE_CURRENT_BINARY_DIR}          # для сгенерированных ui_*.h
)

target_link_libraries(QtCards PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

include(GNUInstallDirs)
install(TARGETS QtCards
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(QtCards)
endif()
