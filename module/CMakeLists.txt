set(CORES
        core/App.ixx
        core/ConfigManager.ixx
        core/Init.ixx
        core/InputSystem.ixx
        core/Window.ixx
)

set(GAMES
        game/cmd/CmdPlayer.ixx
        game/cmd/CmdVariant.ixx
        game/cmd/impl/SinBullet.ixx
        game/comp/BulletComp.ixx
        game/comp/CollisionComp.ixx
        game/comp/DragComp.ixx
        game/comp/PlayerComp.ixx
        game/comp/RenderComp.ixx
        game/comp/SpriteComp.ixx
        game/comp/TagComp.ixx
        game/comp/Comp.ixx
        game/comp/TransformComp.ixx
        game/system/BulletSys.ixx
        game/system/CollisionSys.ixx
        game/system/PlayerSys.ixx
        game/system/SpriteMovementSys.ixx
)

set(GRAPHICS
        graphics/MeshManager.ixx
        graphics/RenderSystem.ixx
        graphics/ShaderManager.ixx
        graphics/Shader.ixx
        graphics/TextureManager.ixx
)

set(RESOURCES
        resources/AudioManager.ixx
)

set(UTILS
        utils/Random.ixx
        utils/Time.ixx
        utils/Config.ixx
        utils/FileManager.ixx
        utils/JsonManager.ixx
)

set(THIRD_PARTY
        thirdparty/glfw.ixx
        thirdparty/stb_image.ixx
        thirdparty/miniaudio.ixx
        thirdparty/opengl.ixx
        thirdparty/spdlog.ixx
        thirdparty/nlohmann_json.ixx
)


# 添加模块文件到目标
target_sources(${PROJECT_NAME}
        PUBLIC
        FILE_SET cxx_modules TYPE CXX_MODULES
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
        FILES ${CORES} ${UTILS} ${GAMES} ${GRAPHICS} ${RESOURCES} ${THIRD_PARTY}
)