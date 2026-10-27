cmake_minimum_required(VERSION 3.10)
project(cv4 C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_executable(cv4 main.c
        src/ch_buf.c
        src/ch_err.c
        src/ch_str.c
        src/ch_list.c
        src/ch_hex.c
)

add_executable(test_str
        tests/test_str.c
        src/ch_str.c
        src/ch_err.c
)

add_executable(test_buf
        tests/test_buf.c
        src/ch_buf.c
        src/ch_err.c
)

add_executable(test_list
        src/ch_list.c
        src/ch_err.c
        tests/test_list.c
)