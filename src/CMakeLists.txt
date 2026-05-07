
set(SOURCES
        main.cpp
        glad.c
        shader.cpp
        model.cpp
        logger.cpp
        stb_image.c
)

set(OBJ_DIR ${PROJECT_BINARY_DIR}/obj)
get_subdirectories(SUBDIRS ${CMAKE_CURRENT_LIST_DIR})

add_executable(app ${SOURCES})

target_include_directories(app
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(app 
    PRIVATE
        dependencies
        opengl32
        #  -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl for linux
)

copy_resources(app
    "${CMAKE_SOURCE_DIR}/shaders"
    "shaders"
)

# copy_resources(app
#     "${CMAKE_SOURCE_DIR}/texture"
#     "texture"
# )

copy_resources(app
    "${CMAKE_SOURCE_DIR}/resources"
    "resources"
)

copy_dlls(app
    "${ASSIMP_ROOT}/bin/libassimp-6.dll"
)