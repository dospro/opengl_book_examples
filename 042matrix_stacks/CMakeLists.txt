cmake_minimum_required(VERSION 3.16)
project(matrix_stack_example)

add_executable(matrix_stack_example main.cpp ../common/error_handler.cpp ../common/matrix.cpp ../common/sdl_app.cpp)
target_link_libraries(matrix_stack_example ${LINK_LIBS})
