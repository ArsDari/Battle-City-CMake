cmake_minimum_required(VERSION 3.28)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_subdirectory("source")

project("Battle-City-CMake")

include_directories("include")

add_executable(Battle-City-${CMAKE_BUILD_TYPE} "source/Main.cpp" "${SRC}")

if(WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Release")
	set_target_properties(Battle-City-${CMAKE_BUILD_TYPE} PROPERTIES WIN32_EXECUTABLE TRUE)
	target_link_libraries(Battle-City-${CMAKE_BUILD_TYPE} PRIVATE SFML::Main)
endif()

target_compile_features(Battle-City-${CMAKE_BUILD_TYPE} PRIVATE cxx_std_17)
target_link_libraries(Battle-City-${CMAKE_BUILD_TYPE} PRIVATE SFML::Window SFML::System SFML::Graphics SFML::Audio SFML::Network)

install(TARGETS "Battle-City-${CMAKE_BUILD_TYPE}" RUNTIME DESTINATION "bin")
install(DIRECTORY "source/Media"
        DESTINATION "bin"
)