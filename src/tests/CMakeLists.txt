cmake_minimum_required(VERSION 3.16)

if(BUILD_TESTS)
    find_package(Qt6 REQUIRED COMPONENTS Core Test Sql)

    file(GLOB_RECURSE CORE_SOURCES "../cpp/core/*.cpp")
    file(GLOB_RECURSE TEST_HEADERS "include/*.h")
    file(GLOB_RECURSE TEST_SOURCES "unit/*.cpp")

    add_executable(CardTests
        ${CORE_SOURCES}
        ${TEST_HEADERS}
        ${TEST_SOURCES}
    )
    
    target_link_libraries(CardTests
        Qt6::Core
        Qt6::Test
        Qt6::Sql
    )
    
    target_include_directories(CardTests PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    )
    
    add_test(NAME CardUnitTests COMMAND CardTests)
    
endif()
