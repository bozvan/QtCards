cmake_minimum_required(VERSION 3.16)

if(BUILD_TESTS)
    find_package(Qt6 REQUIRED COMPONENTS Core Test)
    
    # Включить автоматическое создание MOC
    set(CMAKE_AUTOMOC ON)

    # Создаем исполняемый файл тестов
    add_executable(CardTests
        ../cpp/models/Card.cpp
        ../cpp/models/Deck.cpp
        unit/test_card.cpp
        unit/test_main.cpp
        unit/test_deck.cpp
        include/TestDeck.h
        include/TestCard.h
    )
    
    # Линкуем с нашей библиотекой и Qt
    target_link_libraries(CardTests
        Qt6::Core
        Qt6::Test
    )
    
    # Указываем include директории
    target_include_directories(CardTests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
    )
    
    # Добавляем тест в CTest
    add_test(NAME CardUnitTests COMMAND CardTests)
    
endif()
