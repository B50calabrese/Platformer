cmake_minimum_required(VERSION 3.10)
project(PlatformerProject VERSION 1.0)

# Configuration
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add GameEngine submodule
add_subdirectory(libs/GameEngine)

# Sources
file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")
file(GLOB_RECURSE INCLUDE_FILES "include/*.h" "src/*.h")

# Create Executable
add_executable(PlatformerApp ${SOURCE_FILES} ${INCLUDE_FILES})

# Linking & Includes
target_link_libraries(PlatformerApp PRIVATE GameEngine)
target_include_directories(PlatformerApp
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

# Asset Copying
set(ASSETS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets")
set(ASSETS_BUILD_DIR "${CMAKE_BINARY_DIR}/assets")

# Copy game assets
file(COPY "${ASSETS_DIR}/" DESTINATION "${ASSETS_BUILD_DIR}")

# Copy engine shaders (GameEngine expects them in ${CMAKE_BINARY_DIR}/shaders)
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/libs/GameEngine/src/engine/graphics/shaders/" DESTINATION "${CMAKE_BINARY_DIR}/shaders")

# Define ASSETS_PATH for the application
target_compile_definitions(PlatformerApp PRIVATE ASSETS_PATH="${ASSETS_BUILD_DIR}/")

# Set output directory
set_target_properties(PlatformerApp PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    OUTPUT_NAME "Platformer"
)