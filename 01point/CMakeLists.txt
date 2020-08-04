cmake_minimum_required(VERSION 3.16)
project(point_example)

add_executable(point_example main.cpp ../common/error_handler.cpp ../common/sdl_app.cpp)
target_link_libraries(point_example ${LINK_LIBS})
