cmake_minimum_required(VERSION 2.6)
project(ESA1-Bomberman)

find_package(GTest REQUIRED)

enable_testing()

include_directories(${GTEST_INCLUDE_DIRS} src)

set(SRC src/world.c
        src/event.c
        src/bomb.c
        src/event-handling.c
        src/explosion.c
        src/player.c)

add_executable(test-simulation test/main.cpp ${SRC})
target_link_libraries(test-simulation ${GTEST_LIBRARIES})

add_test(NAME simulation COMMAND ./test-simulation)
