cmake_minimum_required(VERSION 2.8)
project(computer)

include_directories(${PROJECT_SOURCE_DIR}/yaml-cpp/include)
add_subdirectory(yaml-cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES main.cpp logic.cpp logic.h ICLoader.cpp ICLoader.h)
add_executable(computer ${SOURCE_FILES})


target_link_libraries(computer -lm yaml-cpp)
