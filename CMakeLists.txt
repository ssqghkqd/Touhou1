cmake_minimum_required(VERSION 3.25)
project(YourProject VERSION 0.1.0 LANGUAGES CXX)

# 设置 C++ 标准为 23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# 建议也设置扩展模式为 OFF，确保使用标准语法
set(CMAKE_CXX_EXTENSIONS OFF)

# 处理 Conan 依赖
# 首先检查是否在 build 目录下运行，通常是这样
if (NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
  message(STATUS "Downloading conan.cmake from github")
  file(DOWNLOAD "https://github.com/conan-io/cmake-conan/raw/v0.16.1/conan.cmake"
                "${CMAKE_BINARY_DIR}/conan.cmake")
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

# 执行 Conan 安装，根据 conanfile.txt 配置依赖
conan_cmake_autodetect(settings)
conan_cmake_install(
    PATH "${CMAKE_SOURCE_DIR}"
    BUILD missing
    SETTINGS ${settings}
)

# 导入 Conan 生成的文件
# CMakeDeps 生成的文件
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})
# CMakeToolchain 生成的文件
include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake)

# --- 配置本地依赖 (Header-only 库) ---
# glm (Header-only)
add_library(glm INTERFACE)
target_include_directories(glm INTERFACE "${CMAKE_SOURCE_DIR}/external/glm")

# entt (Header-only)
add_library(entt INTERFACE)
target_include_directories(entt INTERFACE "${CMAKE_SOURCE_DIR}/external/entt")

# stb (Header-only, 但通常需要定义一个宏来启用实现)
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE "${CMAKE_SOURCE_DIR}/external/stb")
# 如果你的代码需要包含 stb_image.c 等实现文件，需要单独编译，这里按头文件库处理

# --- 配置本地依赖 (需要编译的库) ---
# GLFW
add_subdirectory(${CMAKE_SOURCE_DIR}/external/glfw)
# 这要求 external/glfw 目录下有自己的 CMakeLists.txt

# Glad
# 假设你的 glad 在 external/glad 下，包含 glad.c 和 glad.h
add_library(glad ${CMAKE_SOURCE_DIR}/external/glad/glad.c)
target_include_directories(glad PUBLIC ${CMAKE_SOURCE_DIR}/external/glad/include)
# 根据你的 glad 文件实际位置调整路径

# SFML
# SFML 比较大，通常推荐用 Conan 管理。本地管理需确保已编译好或包含其 CMake项目
option(SFML_BUILD_AUDIO FALSE) # 根据你的需要关闭某些模块
option(SFML_BUILD_NETWORK FALSE)
add_subdirectory(${CMAKE_SOURCE_DIR}/external/sfml)
# 同样，这要求 external/sfml 是一个完整的 SFML 源码仓库并有 CMakeLists.txt

# 添加你的源代码目录
add_subdirectory(src)