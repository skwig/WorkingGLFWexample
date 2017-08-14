cmake_minimum_required(VERSION 3.8)

set(PROJECT_NAME "OpenGLtest")

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/_install)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCE_FILES main.cpp Shader.cpp Shader.h stb_image.cpp stb_image.h Camera.cpp Camera.h)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

add_subdirectory(glfw)
include_directories(glfw/include)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES})
