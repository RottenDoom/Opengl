include(FetchContent)

# Option: use system GLFW if available, otherwise fallback to bundled
option(USE_SYSTEM_GLFW "Use system-installed GLFW if available" ON)
option(USE_SYSTEM_ASSIMP "Use system-installed ASSIMP if available" ON)

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

if (USE_SYSTEM_ASSIMP)
    find_package(assimp QUIET)
endif()

if (assimp_FOUND)
    message(STATUS "Using system-installed Assimp")
else()
    message(STATUS "Using bundled Assimp (thirdparty/assimp)")
    add_subdirectory(thirdparty/assimp)
endif()

set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)

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
)