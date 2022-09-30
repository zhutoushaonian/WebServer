cmake_minimum_required(VERSION 3.10)

project(HttpServer)

set(CMAKE_CXX_STANDARD 11)

# 设置生成server的位置
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_subdirectory(src)
