cmake_minimum_required(VERSION 3.14)

# Project declaration
project(ShoeEngine VERSION 0.1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Download CPM.cmake
set(CPM_DOWNLOAD_VERSION 0.38.6)
set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
if(NOT (EXISTS ${CPM_DOWNLOAD_LOCATION}))
    message(STATUS "Downloading CPM.cmake...")
    file(DOWNLOAD
        https://github.com/cpm-cmake/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake
        ${CPM_DOWNLOAD_LOCATION}
    )
endif()
include(${CPM_DOWNLOAD_LOCATION})

# Add source files
set(SOURCES
    src/main.cpp
    src/graphics/Window.cpp
    src/core/DataManager.cpp
)

# Add SFML dependency
CPMAddPackage(
    NAME SFML
    GITHUB_REPOSITORY SFML/SFML
    GIT_TAG 2.6.1
    OPTIONS
        "SFML_BUILD_AUDIO TRUE"
        "SFML_BUILD_NETWORK TRUE"
        "SFML_BUILD_GRAPHICS TRUE"
        "SFML_BUILD_WINDOW TRUE"
)

# Add nlohmann/json dependency
CPMAddPackage(
    NAME nlohmann_json
    VERSION 3.11.3
    GITHUB_REPOSITORY nlohmann/json
    OPTIONS
        "JSON_BuildTests OFF"
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link SFML
target_link_libraries(${PROJECT_NAME} PRIVATE
    sfml-system
    sfml-window
    sfml-graphics
    sfml-audio
    sfml-network
    nlohmann_json::nlohmann_json
)

# Set output directories
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Copy SFML DLLs for main executable
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:sfml-system>
        $<TARGET_FILE:sfml-window>
        $<TARGET_FILE:sfml-graphics>
        $<TARGET_FILE:sfml-audio>
        $<TARGET_FILE:sfml-network>
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

# Copy data directory to build output
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/data
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/data
)

# Enable testing
enable_testing()

# Add Google Test
CPMAddPackage(
    NAME GoogleTest
    GITHUB_REPOSITORY google/googletest
    GIT_TAG v1.14.0
    VERSION 1.14.0
    OPTIONS
        "INSTALL_GTEST OFF"
        "gtest_force_shared_crt ON"
)

# Add test executable
set(TEST_SOURCES
    tests/graphics/WindowTests.cpp
    tests/core/DataManagerTests.cpp
    src/graphics/Window.cpp
    src/core/DataManager.cpp
)

add_executable(${PROJECT_NAME}_tests ${TEST_SOURCES})
target_link_libraries(${PROJECT_NAME}_tests PRIVATE
    GTest::gtest_main
    sfml-system
    sfml-window
    sfml-graphics
    nlohmann_json::nlohmann_json
)

target_include_directories(${PROJECT_NAME}_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

add_test(NAME ${PROJECT_NAME}_tests COMMAND ${PROJECT_NAME}_tests)

# Set test output directory
set_target_properties(${PROJECT_NAME}_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Copy SFML DLLs for test executable
add_custom_command(TARGET ${PROJECT_NAME}_tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:sfml-system>
        $<TARGET_FILE:sfml-window>
        $<TARGET_FILE:sfml-graphics>
        $<TARGET_FILE:sfml-audio>
        $<TARGET_FILE:sfml-network>
        $<TARGET_FILE_DIR:${PROJECT_NAME}_tests>
)
