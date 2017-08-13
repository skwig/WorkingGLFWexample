cmake_minimum_required(VERSION 3.8)

set(PROJECT_NAME
        "OpenGLtest"
        )

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

find_package(OpenGL REQUIRED)

add_subdirectory(glfw)
include_directories(glfw/include)
target_link_libraries(${PROJECT_NAME} glfw ${OPENGL_LIBRARIES} ${GLFW_LIBRARIES})
