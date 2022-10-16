
project(UnitTests)

find_package(Catch2 3 REQUIRED)

add_executable(
    tests
    tests.cpp
    ../src/Order.h
    ../src/Order.cpp
    ../src/OrderBook.h
    ../src/OrderBook.cpp
    ../src/PriceLevel.h
    ../src/PriceLevel.cpp
    )

target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
target_include_directories(tests PRIVATE "../src")
target_compile_definitions(tests PRIVATE UNIT_TEST)

