cmake_minimum_required(VERSION 3.16)
project(matrix_stack_example)

add_executable(textures_example main.cpp ../common/error_handler.cpp ../common/matrix.cpp ../common/sdl_app.cpp)
target_link_libraries(textures_example ${LINK_LIBS})
