# src/CMakeLists.txt

# 按模块分组源文件
set(CORE_SOURCES
        core/App.cpp
        core/Init.cpp
        core/InputSystem.cpp
        core/Window.cpp
        core/ConfigManager.cpp
)

set(GAME_SOURCES
        game/system/BulletSys.cpp
        game/system/CollisionSys.cpp
        game/system/PlayerSys.cpp
        game/system/SpriteMovementSys.cpp
        game/cmd/impl/SinBullet.cpp
        game/cmd/CmdPlayer.cpp
)

set(GRAPHICS_SOURCES
        graphics/MeshManager.cpp
        graphics/RenderSystem.cpp
        graphics/Shader.cpp
        graphics/ShaderManager.cpp
        graphics/TextureManager.cpp
)

set(UTILS_SOURCES
        utils/FileManager.cpp
        utils/JsonManager.cpp
        utils/Random.cpp
        utils/Time.cpp
)

set(RESOURCES_SOURCES
        resources/AudioManager.cpp
)

set(THIRDPARTY_SOURCES
        thirdparty/stb_image.cpp
        thirdparty/miniaudio.cpp
)

# 合并所有源文件
set(SOURCES
        main.cpp
        ${CORE_SOURCES}
        ${GAME_SOURCES}
        ${GRAPHICS_SOURCES}
        ${UTILS_SOURCES}
        ${RESOURCES_SOURCES}
        ${THIRDPARTY_SOURCES}
)


target_sources(${PROJECT_NAME}
        PRIVATE
        ${SOURCES}
)

# 包含目录 - 确保头文件能被找到
target_include_directories(${PROJECT_NAME} PRIVATE
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/third_party>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/third_party/entt/src>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/third_party/spdlog/include>
)

# 编译器配置
set_target_warnings(${PROJECT_NAME})
set_cpp_standard(${PROJECT_NAME} 23)

# 链接库
target_link_libraries(${PROJECT_NAME} PRIVATE
        glfw
        glad
        -lc++
        -lc++abi
)

# 编译定义
target_compile_definitions(${PROJECT_NAME} PRIVATE
        $<$<BOOL:${APPLE}>:GL_SILENCE_DEPRECATION>
)


# 可选：为每个模块设置源码组（在IDE中更好看）
source_group("Core" FILES ${CORE_SOURCES})
source_group("Game Systems" FILES ${GAME_SOURCES})
source_group("Graphics" FILES ${GRAPHICS_SOURCES})
source_group("Utils" FILES ${UTILS_SOURCES})
source_group("Resources" FILES ${RESOURCES_SOURCES})
source_group("Main" FILES main.cpp)