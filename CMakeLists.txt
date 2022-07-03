cmake_minimum_required(VERSION 3.22)
project(SineWave)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)


if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug
            CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

configure_file(config.h.in config.h)

find_package(SFML 2.5
        COMPONENTS
        system window graphics network audio REQUIRED)


option(USE_INSTALL_RESOURCE_PATH
        "Set resource path to install location" OFF)
if(NOT USE_INSTALL_RESOURCE_PATH)
    set(RESOURCE_PATH "${PROJECT_SOURCE_DIR}/content/")
else()
    set(RESOURCE_PATH
            "${CMAKE_INSTALL_PREFIX}/share/${CMAKE_PROJECT_NAME}/content/")
endif()



add_executable(SineWave main.cpp Renderer.cpp Renderer.hpp)
target_include_directories(SineWave PRIVATE "${PROJECT_BINARY_DIR}")
target_link_libraries(SineWave sfml-graphics)
install(TARGETS SineWave DESTINATION bin)


