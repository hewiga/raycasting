cmake_minimum_required(VERSION 3.16)
project(Raycasting)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


include_directories(${CMAKE_SOURCE_DIR}/dependencies/include)
link_directories(${CMAKE_SOURCE_DIR}/dependencies/lib)

file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME}
    optimized sfml-graphics
    optimized sfml-window
    optimized sfml-system
    optimized sfml-audio

    debug     sfml-graphics-d
    debug     sfml-window-d
    debug     sfml-system-d
    debug     sfml-audio-d
)



if (WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/dependencies/bin"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
    )
endif()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/assets"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
)