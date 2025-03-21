cmake_minimum_required(VERSION 3.31)

project(
    LinkedList
    VERSION 1.0.0
    DESCRIPTION "Simple LinkedList"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS On)

include(GNUInstallDirs)

add_subdirectory(src)

option(BUILD_TESTS "Build tests" ON)
option(BUILD_EXAMPLES "Build example executables" ON)

if (BUILD_TESTS)
    message(STATUS "Building tests...")
    add_subdirectory(tests)
endif()

if (BUILD_EXMAMPLES)
    message(STATUS "Building examples...")
    add_subdirectory(examples)
endif()

install(
    FILES cmake/linkedlist_config.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/LinkedList/cmake
)

install(
    EXPORT ${CMAKE_PROJECT_NAME}
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/LinkedList/cmake
)
