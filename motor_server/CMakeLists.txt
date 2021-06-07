cmake_minimum_required(VERSION 3.2)

set(HAVE_FLAG_SEARCH_PATHS_FIRST 0)
project( motor_server )
set(CMAKE_C_COMPILER "arm-linux-gnueabihf-clang")

set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/motor_server.c
    )

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME}
    PUBLIC usr/include
)

target_link_directories(${PROJECT_NAME}
    PUBLIC usr/lib
    PUBLIC usr/lib/arm-linux-gnueabihf
)

target_link_libraries(${PROJECT_NAME} pigpio pthread)
