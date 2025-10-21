add_executable(Battle-City-${CMAKE_BUILD_TYPE} "main.cpp")

if(WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Release")
	set_target_properties(Battle-City-${CMAKE_BUILD_TYPE} PROPERTIES WIN32_EXECUTABLE TRUE)
	target_link_libraries(Battle-City-${CMAKE_BUILD_TYPE} PRIVATE SFML::Main)
endif()

target_compile_features(Battle-City-${CMAKE_BUILD_TYPE} PRIVATE cxx_std_17)
target_link_libraries(Battle-City-${CMAKE_BUILD_TYPE} PRIVATE SFML::Window SFML::System SFML::Graphics SFML::Audio SFML::Network)