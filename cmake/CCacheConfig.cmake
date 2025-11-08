# CCache配置
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}" CACHE STRING "C++ compiler launcher")
    message(STATUS "Using ccache: ${CCACHE_PROGRAM}")
endif()