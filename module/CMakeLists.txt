set(CORES
        core/test.ixx
)

set(UTILS
        utils/Random.ixx
)

set(THIRD_PARTY
        thirdparty/glfw.ixx
        thirdparty/stb_image.ixx
        thirdparty/miniaudio.ixx
        thirdparty/entt.ixx
        thirdparty/opengl.ixx
        thirdparty/glm.ixx
        thirdparty/spdlog.ixx
        thirdparty/nlohmann_json.ixx
)


# 添加模块文件到目标
target_sources(${PROJECT_NAME}
        PUBLIC
        FILE_SET cxx_modules TYPE CXX_MODULES
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
        FILES ${CORES} ${UTILS} ${THIRD_PARTY}
)