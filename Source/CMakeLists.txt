add_executable("Battle-City" "Main.cpp")
target_include_directories("Battle-City" PRIVATE "${PROJECT_SOURCE_DIR}/Include/")
target_sources("Battle-City" PRIVATE
    "Animation.cpp"
    "Application.cpp"
    "Button.cpp"
    "Command.cpp"
    "CommandQueue.cpp"
    "Component.cpp"
    "Container.cpp"
    "DataTables.cpp"
    "Entity.cpp"
    "GameOverState.cpp"
    "GameState.cpp"
    "Label.cpp"
    "LoadingState.cpp"
    "MenuState.cpp"
    "PauseState.cpp"
    "Pickup.cpp"
    "Player.cpp"
    "PlayerData.cpp"
    "Projectile.cpp"
    "RectangleNode.cpp"
    "SceneNode.cpp"
    "SettingsState.cpp"
    "SpriteNode.cpp"
    "State.cpp"
    "StateStack.cpp"
    "StatisticsNode.cpp"
    "Tank.cpp"
    "TextNode.cpp"
    "Tilemap.cpp"
    "TileNode.cpp"
    "Utility.cpp"
    "World.cpp"
)

target_compile_features("Battle-City" PRIVATE cxx_std_17)
target_link_libraries("Battle-City" PRIVATE SFML::Window SFML::System SFML::Graphics SFML::Audio SFML::Network)

if(WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Release")
    set_target_properties("Battle-City" PROPERTIES WIN32_EXECUTABLE TRUE)
    target_link_libraries("Battle-City" PRIVATE SFML::Main)
endif()