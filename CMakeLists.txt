cmake_minimum_required(VERSION 3.10)

project("Hirie"
    VERSION 1.0.0
    DESCRIPTION "3d graphic library based on SDL"
    LANGUAGES C CXX ASM_NASM
)

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

find_package(SDL2 REQUIRED)

file(GLOB_RECURSE HIRIE_FILES CONFIGURE_DEPENDS src/*.cpp src/*.hpp)


add_library(${PROJECT_NAME} STATIC ${HIRIE_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
        ${SDL2_INCLUDE_DIR}
)

target_compile_options(${PROJECT_NAME} 
    PRIVATE 
    -O3
    -g
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE 
    SDL2
)

install(TARGETS ${PROJECT_NAME} ARCHIVE DESTINATION lib)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION include/hirie)