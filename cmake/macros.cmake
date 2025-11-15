## Macro for printing CMake variables ##
macro(print var)
    message("${var} = ${${var}}")
endmacro()

## Get a list of subdirectories (single level) under a given directory
macro(get_subdirectories result curdir)
    file(GLOB children RELATIVE ${curdir} ${curdir}/*)
    set(dirlist "")
    foreach(child ${children})
        if(IS_DIRECTORY ${curdir}/${child})
            list(APPEND dirlist ${child})
        endif()
    endforeach()
    set(${result} ${dirlist})
endmacro()

# Copy a directory to the target's runtime output directory after build
macro(copy_resources TARGET_NAME SRC_DIR DEST_SUBDIR)
    add_custom_command(
        TARGET ${TARGET_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${SRC_DIR}"
            "$<TARGET_FILE_DIR:${TARGET_NAME}>/${DEST_SUBDIR}"
        COMMENT "Copying ${SRC_DIR} to $<TARGET_FILE_DIR:${TARGET_NAME}>/${DEST_SUBDIR}"
    )
endmacro()