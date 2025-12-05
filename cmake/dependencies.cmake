include(FetchContent)

# Option: use system GLFW if available, otherwise fallback to bundled
option(USE_SYSTEM_GLFW "Use system-installed GLFW if available" ON)

if (USE_SYSTEM_GLFW)
    find_package(glfw3 QUIET)
endif()

if (glfw3_FOUND)
    message(STATUS "Using system-installed GLFW")
else()
    message(STATUS "Using bundled GLFW (thirdparty/glfw)")
    add_subdirectory(thirdparty/glfw)
endif()

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

set(ASSIMP_ROOT "${CMAKE_SOURCE_DIR}/external/assimp")
set(BUILD_ASSIMP_SHARED "Build the assimp library as shared" ON) # might add static or dynamic linking using the third-party submodule later.

add_library(assimp SHARED IMPORTED)

set_target_properties(assimp PROPERTIES
    IMPORTED_LOCATION             "${ASSIMP_ROOT}/bin/libassimp-6.dll"
    IMPORTED_IMPLIB               "${ASSIMP_ROOT}/lib/libassimp.dll.a"
    INTERFACE_INCLUDE_DIRECTORIES "${ASSIMP_ROOT}/include"
)

# GLM
FetchContent_Declare(
    glm
    GIT_REPOSITORY "https://github.com/g-truc/glm.git"
    GIT_TAG 1.0.1
)

FetchContent_MakeAvailable(glm)

add_library(dependencies INTERFACE)

target_link_libraries(dependencies
    INTERFACE
        glfw
        glm
        assimp
)

target_include_directories(dependencies
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glad/include
        ${ASSIMP_ROOT}/include
)