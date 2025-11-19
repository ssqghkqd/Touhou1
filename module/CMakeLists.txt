set(UTILS_MODULES
        utils/Random.ixx
)

set(ALL_MODULES
        ${ULILS_MODULES}
)

#target_compile_options(${PROJECT_NAME} PRIVATE -fmodules -fuse-ld=lld)

# 添加模块文件到目标
#target_sources(${PROJECT_NAME}
        #PUBLIC
        #FILE_SET cxx_modules TYPE CXX_MODULES
        #BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
        #FILES ${ALL_MODULES}
#)