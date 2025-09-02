# 列出你的所有源文件
add_executable(${PROJECT_NAME}
        main.cpp
        # 其他.cpp文件...
)

# 包含你的头文件目录
target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/thirdparty
)

# 只需要链接需要编译的库，头文件库已经通过包含目录解决了
target_link_libraries(${PROJECT_NAME} PRIVATE
        glad      # GLAD需要链接
        glfw      # GLFW需要链接
        sfml-system
        sfml-audio
)


# Windows特定的系统库
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE
            opengl32
            gdi32
    )
endif()