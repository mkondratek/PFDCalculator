cmake_minimum_required(VERSION 3.7)
project(PFDCalculator)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(lib/googletest-master)
include_directories(lib/googletest-master/googletest/include)
include_directories(lib/googletest-master/googlemock/include)

set(SOURCE_FILES main.cpp tests/gt_Coeff.cpp tests/gt_Poly.cpp GreekLetters.h Poly.tpp Poly.h)
add_executable(PFDCalculator ${SOURCE_FILES})
target_link_libraries(PFDCalculator gtest gtest_main)
