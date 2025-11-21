cmake_minimum_required(VERSION 3.16)
project(CG CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(WIN32)

    set(PLATFORM_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/windows)

    set(GLFW_INCLUDE_DIRS ${GLFW_INCLUDE_DIR})

    set(GLFW_LIBRARIES ${GLFW_LIBRARY} gdi32)

    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")

else()

    set(PLATFORM_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/linux)

    find_package(glfw3 REQUIRED)

    set(GLFW_LIBRARIES glfw)

endif()

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/Ds/include
        ${CMAKE_CURRENT_SOURCE_DIR}/Ds/externals/include
        ${GLFW_INCLUDE_DIRS}
)

add_executable(CG
        src/Main.cpp
        src/MainWindow.cpp
)

find_package(OpenGL REQUIRED)

target_link_libraries(CG PRIVATE
        ${PLATFORM_LIB_DIR}/libcg.a
        ${PLATFORM_LIB_DIR}/libgl3w.a
        ${GLFW_LIBRARIES}
        OpenGL::GL
)