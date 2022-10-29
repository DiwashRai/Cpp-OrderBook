
project(Quantcup)

#[[
add_executable(
    score
    score.cpp
    limits.h
    types.h
    score_feed.h
    )
]]

add_executable(
    quantcupTests
    test.cpp
    ../src/Order.h
    ../src/Order.cpp
    ../src/OrderBook.h
    ../src/OrderBook.cpp
    ../src/PriceLevel.h
    ../src/PriceLevel.cpp
    )

add_executable(
    quantcupScore
    score.cpp
    ../src/Order.h
    ../src/Order.cpp
    ../src/OrderBook.h
    ../src/OrderBook.cpp
    ../src/PriceLevel.h
    ../src/PriceLevel.cpp
    )

target_compile_definitions(quantcupTests PRIVATE QUANTCUP_TEST)
target_include_directories(quantcupTests PRIVATE ../src)
target_include_directories(quantcupScore PRIVATE ../src)

