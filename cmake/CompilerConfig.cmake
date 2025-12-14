# 编译器警告配置
function(set_target_warnings target)
    target_compile_options(${target} PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Wshadow
            -Wunused
            -Werror
    )
endfunction()

# C++标准配置
function(set_cpp_standard target standard)
    set_target_properties(${target} PROPERTIES
            CXX_STANDARD ${standard}
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF
    )
endfunction()