cmake_minimum_required(VERSION 3.16)
project(multiple_cubes_example)

add_executable(multiple_cubes_example main.cpp ../common/error_handler.cpp ../common/matrix.cpp ../common/sdl_app.cpp)
target_link_libraries(multiple_cubes_example ${LINK_LIBS})
