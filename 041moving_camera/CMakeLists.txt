cmake_minimum_required(VERSION 3.16)
project(moving_camera_example)

add_executable(moving_camera_example main.cpp ../common/error_handler.cpp ../common/matrix.cpp ../common/sdl_app.cpp)
target_link_libraries(moving_camera_example ${LINK_LIBS})
