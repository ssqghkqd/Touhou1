cmake_minimum_required(VERSION 3.28)
project(Touhou1 LANGUAGES CXX)

# 修复缺失的变量
set(CMAKE_C_COMPILE_OBJECT "<CMAKE_C_COMPILER> <DEFINES> <INCLUDES> <FLAGS> -o <OBJECT> -c <SOURCE>")
set(CMAKE_C_CREATE_STATIC_LIBRARY "<CMAKE_AR> qc <TARGET> <OBJECTS>")
set(CMAKE_CXX_COMPILE_OBJECT "<CMAKE_CXX_COMPILER> <DEFINES> <INCLUDES> <FLAGS> -o <OBJECT> -c <SOURCE>")
set(CMAKE_CXX_CREATE_STATIC_LIBRARY "<CMAKE_AR> qc <TARGET> <OBJECTS>")

# 基础配置
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
    message(${CCACHE_PROGRAM})
endif()


# 第三方库路径
set(THIRDPARTY_DIR ${CMAKE_SOURCE_DIR}/thirdparty)

# 包含目录（完全还原你的原始配置）
include_directories(
        ${CMAKE_SOURCE_DIR}/include
        ${THIRDPARTY_DIR}/glm
        ${THIRDPARTY_DIR}/entt/src
        ${THIRDPARTY_DIR}/stb
        ${THIRDPARTY_DIR}/glad
        ${THIRDPARTY_DIR}/soloud/include
        ${THIRDPARTY_DIR}/GLFW/include
        ${THIRDPARTY_DIR}/miniaudio
        ${THIRDPARTY_DIR}
        ${THIRDPARTY_DIR}/nlohmann_json
        /usr/lib/llvm-21/include/c++/v1
)

# 编译 GLFW（静态库）
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Disable docs" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Disable tests" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Disable examples" FORCE)
add_subdirectory(${THIRDPARTY_DIR}/GLFW)

# 编译 GLAD
file(GLOB GLAD_SOURCES ${THIRDPARTY_DIR}/glad/glad.c)
add_library(glad STATIC ${GLAD_SOURCES})

add_compile_options(
        -stdlib=libc++
        -nostdinc++  # 禁止默认的 C++ 头文件搜索路径
)

link_libraries(c++ c++abi)

# 项目源文件
file(GLOB_RECURSE SOURCES src/*.cpp src/*.c)

# 添加可执行文件
add_executable(${PROJECT_NAME} ${SOURCES})

# Windows 下必需的链接库（精简版）
target_link_libraries(${PROJECT_NAME}
        glfw           # GLFW 静态库
        glad           # GLAD
)

# 全局宏定义（保留原始配置）
target_compile_definitions(${PROJECT_NAME} PRIVATE
)

# SoLoud 静态库定义（保留原始配置）

# GLAD 的 OpenGL 版本（保留原始配置）
target_compile_definitions(glad PRIVATE
        GLAD_GL_VERSION=3_3
)


