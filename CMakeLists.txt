cmake_minimum_required(VERSION 4.3)
project(cv4 C)

set(CMAKE_C_STANDARD 11)


add_executable(cv4
        src/ch_buf.c
        include/ch_buf.h
        src/ch_err.c
        include/ch_err.h
        include/ch_str.h
        src/ch_str.c
        tests/ch_test.h
        tests/test_str.c
        src/ch_list.c
        include/ch_list.h)
