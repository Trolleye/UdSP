cmake_minimum_required(VERSION 4.3)
project(cv4 C)

set(CMAKE_C_STANDARD 11)

include_directories(cv2)
include_directories(cv4)

add_executable(cv4
        cv4/src/ch_buf.c
        cv4/include/ch_buf.h
        cv4/src/ch_err.c
        cv4/include/ch_err.h
        cv4/include/ch_str.h
        cv4/src/ch_str.c
        cv4/tests/ch_test.h
        cv4/tests/test_str.c)
