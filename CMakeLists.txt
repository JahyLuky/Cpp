cmake_minimum_required(VERSION 3.21.1)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra -Wno-long-long ")
#set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
#set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")

set(CMAKE_CXX_STANDARD 17)

project(PA2_hw1)
add_executable(PA2_hw1
        progtest/hw_01/test.cpp)

project(PA2_hw2)
add_executable(PA2_hw2
        progtest/hw_02/test.cpp)

project(PA2_hw3)
add_executable(PA2_hw3
        progtest/hw_03/test.cpp)

project(PA2_hw4)
add_executable(PA2_hw4
        progtest/hw_04/test.cpp)
