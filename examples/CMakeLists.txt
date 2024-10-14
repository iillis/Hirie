cmake_minimum_required(VERSION 3.10)
project(HirieExample)

file(GLOB SOURCES "./*.cpp")

find_package(SDL2 REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(/home/blob/Documents/3d-Engine/include)

set(HIRIE_LIB "${CMAKE_SOURCE_DIR}/../build/bin/libHirie.a")
if(NOT EXISTS ${HIRIE_LIB})
    message(FATAL_ERROR "Hirie library not found at: ${HIRIE_LIB}")
endif()

foreach(SOURCE ${SOURCES})
    get_filename_component(EXECUTABLE_NAME ${SOURCE} NAME_WE)
    add_executable(${EXECUTABLE_NAME} ${SOURCE})
    target_link_libraries(${EXECUTABLE_NAME} SDL2 ${HIRIE_LIB})
endforeach()

message(STATUS "Hirie library linked from: ${HIRIE_LIB}")
message(STATUS "Source Directory: ${CMAKE_SOURCE_DIR}")
message(STATUS "Binary Directory: ${CMAKE_BINARY_DIR}")
