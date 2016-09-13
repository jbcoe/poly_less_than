cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)

include_directories(SYSTEM external/catch/include)
include_directories(include)

add_subdirectory(tests)

enable_testing()

add_test(test_poly_less_than ${CMAKE_BINARY_DIR}/tests/test_poly_less_than -s)
