
project(Quantcup)

add_executable(
    score
    score.cpp
    limits.h
    types.h
    score_feed.h
    engine.h
    engine.cpp
)

add_executable(
    test
    test.cpp
    limits.h
    types.h
    score_feed.h
    engine.h
    engine.cpp
)
