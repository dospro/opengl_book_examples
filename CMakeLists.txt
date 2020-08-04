cmake_minimum_required(VERSION 3.16)
project(opengl_shaders)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${opengl_shaders_SOURCE_DIR}/cmake/Modules")
set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

include_directories(${SDL2_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIR})
include_directories(/usr/local/Cellar/glew/2.1.0_1/include/)
set(GLEW_LIBRARY "/usr/local/lib/libGLEW.a;/usr/local/lib/libGLEW.dylib")
message("${GLEW_LIBRARY}")
set(LINK_LIBS ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${OPENGL_LIBRARIES} ${GLEW_LIBRARY})

add_subdirectory(01point)
add_subdirectory(02cube)
add_subdirectory(03cubes)
