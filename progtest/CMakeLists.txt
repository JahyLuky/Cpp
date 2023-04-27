cmake_minimum_required(VERSION 3.22)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra -Wno-long-long ")
#set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
#set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")

set(CMAKE_CXX_STANDARD 17)

project(PA2_hw1-Huffman)
add_executable(PA2_hw1-Huffman
        1-Huffman_code/test.cpp)

project(PA2_hw1-Fib)
add_executable(PA2_hw1-Fib
        1-Fibonacci_code/test.cpp)

project(PA2_hw2-Control)
add_executable(PA2_hw2-Control
        2-Control_report/test.cpp)

project(PA2_hw2-Employee)
add_executable(PA2_hw2-Employee
        2-Employee_records/test.cpp)

project(PA2_hw3-Date)
add_executable(PA2_hw3-Date
        3-Date/test.cpp)

project(PA2_hw3-Intervals)
add_executable(PA2_hw3-Intervals
        3-Intervals/test.cpp)

project(PA2_hw4-Mail)
add_executable(PA2_hw4-Mail
        4-Mail_server/test.cpp)

project(PA2_hw4-File)
add_executable(PA2_hw4-File
        4-File_with_versions/test.cpp)

project(PA2_hw5-Supermarket)
add_executable(PA2_hw5-Supermarket
        5-Supermarket/test.cpp)

project(PA2_hw5-Checking)
add_executable(PA2_hw5-Checking
        5-Checking_the_inspection_report/test.cpp)

project(PA2_hw6-Dialog_box)
add_executable(PA2_hw6-Dialog_box
        6-Dialog_box/test.cpp)

project(PA2_hw7-Result_sheet)
add_executable(PA2_hw7-Result_sheet
        7-Result_sheet/test.cpp)
