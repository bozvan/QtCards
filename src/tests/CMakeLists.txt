cmake_minimum_required(VERSION 3.16)

if(BUILD_TESTS)
    find_package(Qt6 REQUIRED COMPONENTS Test)
    
    # Создаем исполняемый файл тестов
    add_executable(CardTests
        ../cpp/models/Card.cpp
        unit/test_card.cpp
    )
    
    # Линкуем с нашей библиотекой и Qt
    target_link_libraries(CardTests
        Qt6::Core
        Qt6::Test
    )
    
    # Указываем include директории
    target_include_directories(CardTests PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src
    )
    
    # Добавляем тест в CTest
    add_test(NAME CardUnitTests COMMAND CardTests)
    
endif()
