cmake_minimum_required(VERSION 3.16)
project(QtCards VERSION 0.1 LANGUAGES CXX)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Sql)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/src/ui)

option(BUILD_TESTS "Build tests" ON)

file(GLOB_RECURSE SOURCES "src/cpp/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h")
file(GLOB_RECURSE FORMS "src/ui/*.ui")

qt_add_executable(QtCards
    ${SOURCES}
    ${HEADERS}
    ${FORMS}
)

target_include_directories(QtCards PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(QtCards PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Sql
)

if(BUILD_TESTS)
    add_subdirectory(src/tests) # Подключит src/tests/CMakeLists.txt
endif()

include(GNUInstallDirs)
if(WIN32)
    # Windows:
    install(TARGETS QtCards
        RUNTIME DESTINATION .
        LIBRARY DESTINATION .
        ARCHIVE DESTINATION lib
    )
else()
    # Unix/macOS:
    include(GNUInstallDirs)
    install(TARGETS QtCards
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        BUNDLE DESTINATION .
    )
endif()

