cmake_minimum_required(VERSION 3.10)



add_executable (httpserver buffer.cpp http_conn.cpp locker.cpp lst_timer.cpp server.cpp)
find_package (Threads)
target_link_libraries(httpserver ${CMAKE_THREAD_LIBS_INIT})
include_directories (${PROJECT_SOURCE_DIR}/include)

