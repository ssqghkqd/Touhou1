# src/CMakeLists.txt

set(THIRD_PARTY_SOURCES
        thirdparty/miniaudio.cpp
        thirdparty/stb_image.cpp
)

# 合并所有源文件
set(SOURCES
        main.cpp
        ${THIRD_PARTY_SOURCES}
)
# ----------------------------------------------

# 模块接口文件
set(THIRD_PARTY
        thirdparty/entt.cppm
        thirdparty/glfw.cppm
        thirdparty/miniaudio.cppm
        thirdparty/nlohmann_json.cppm
        thirdparty/spdlog.cppm
        thirdparty/stb_image.cppm
)

set(CORES
        core/App.cppm
        core/Window.cppm
        core/Init.cppm
        core/core.cppm
        core/AppLogicSys.cppm
        core/InputSystem.cppm
)

set(UTILS
        utils/utils.cppm
        utils/FileUtils.cppm
        utils/Math.cppm
        utils/Time.cppm
        utils/Random.cppm
)

set(RESOURCES
        resources/AudioManager.cppm
        resources/resources.cppm
)

set(DEFS
        defs/Config.cppm
        defs/defs.cppm
        defs/Error.cppm
        defs/Events.cppm
        defs/Input.cppm
)

set(GAMES
        #game/cmd/game.cmd.cppm
        #game/cmd/CmdPlayer.cppm
        #game/cmd/impl/SinBullet.cppm
        #game/cmd/impl/game.cmd.impl.cppm

        game/comp/SpriteComp.cppm
        game/comp/game.comp.cppm
        game/comp/Tag.cppm
)

set(GRAPHICS
        graphics/vk/vk.cppm
        graphics/vk/VKContext.cppm
        graphics/vk/utils.cppm
        #graphics/vk/VKPipeline.cppm
)

target_sources(${PROJECT_NAME}
        PRIVATE
        ${SOURCES}
)

target_sources(${PROJECT_NAME}
        PUBLIC
        FILE_SET cxx_modules TYPE CXX_MODULES
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/third_party/glm
        FILES ${CORES} ${UTILS} ${GAMES} ${GRAPHICS} ${RESOURCES} ${DEFS} ${THIRD_PARTY} ../third_party/glm/glm.cppm
)


# 包含目录 - 确保头文件能被找到
target_include_directories(${PROJECT_NAME} PRIVATE
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/third_party>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/third_party/entt/src>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/third_party/spdlog/include>
)

# 编译器配置
set_target_warnings(${PROJECT_NAME})
set_cpp_standard(${PROJECT_NAME} 23)
no_rtti(${PROJECT_NAME})

find_package(glfw3 3.4 REQUIRED)
find_package(Vulkan REQUIRED)

# 链接库
target_link_libraries(${PROJECT_NAME} PRIVATE
        glfw
        Vulkan::Vulkan
        c++
        c++abi
)