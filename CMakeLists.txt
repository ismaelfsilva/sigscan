cmake_minimum_required(VERSION 3.16)
project(sigscan LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(sigscan src/sigscan.cpp)
target_include_directories(sigscan PUBLIC include)

add_executable(sigscan_cli src/main.cpp)
set_target_properties(sigscan_cli PROPERTIES OUTPUT_NAME sigscan)
target_link_libraries(sigscan_cli PRIVATE sigscan)

enable_testing()
add_executable(test_sigscan tests/test_sigscan.cpp)
target_link_libraries(test_sigscan PRIVATE sigscan)
add_test(NAME test_sigscan COMMAND test_sigscan)
