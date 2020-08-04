cmake_minimum_required(VERSION 3.16)
project(cube_example)

add_executable(cube_example main.cpp ../common/error_handler.cpp ../common/matrix.cpp ../common/sdl_app.cpp)
target_link_libraries(cube_example ${LINK_LIBS})
