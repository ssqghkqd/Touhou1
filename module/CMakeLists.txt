set(UTILS_MODULES
        utils/Random.ixx
)



# 添加模块文件到目标
target_sources(${PROJECT_NAME}
        PUBLIC
        FILE_SET cxx_modules TYPE CXX_MODULES
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
        FILES ${UTILS_MODULES}
)