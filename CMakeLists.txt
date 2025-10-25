cmake_minimum_required(VERSION 3.28)

project("Battle-City-CMake")

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_subdirectory("Source/")

install(TARGETS "Battle-City" DESTINATION "${CMAKE_INSTALL_PREFIX}")
install(DIRECTORY "Media" DESTINATION "${CMAKE_INSTALL_PREFIX}")