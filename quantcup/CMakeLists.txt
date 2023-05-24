
project(Quantcup)

#[[
add_executable(
    score
    score.cpp
    limits.h
    score_feed.h
    )
]]

add_executable(
    quantcupTests
    test.cpp
    ../src/OrderBook.h
    ../src/OrderBook.cpp
    ../src/PriceLevel.h
)

add_executable(
    quantcupScore
    score.cpp
    ../src/OrderEntry.h
    ../src/OrderBook.h
    ../src/OrderBook.cpp
    ../src/PriceLevel.h
)

target_compile_definitions(quantcupTests PRIVATE QUANTCUP_TEST)
target_include_directories(quantcupTests PRIVATE ../src)
target_include_directories(quantcupScore PRIVATE ../src)

