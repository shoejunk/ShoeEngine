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

# Find all source files recursively
file(GLOB_RECURSE SOURCES
    "src/*.cpp"
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

# Set include directories for main executable
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
)

# Set output directories for all targets
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Set Visual Studio debugger working directory
if(MSVC)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    )
endif()

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

# Copy sample.json to build output
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/data/sample.json
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/data/sample.json
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

# Find all test source files recursively
file(GLOB_RECURSE TEST_SOURCES
    "tests/*.cpp"
)

# Add source files to test sources as well
list(APPEND TEST_SOURCES ${SOURCES})

# Exclude main.cpp from test sources
list(FILTER TEST_SOURCES EXCLUDE REGEX ".*main\\.cpp$")

add_executable(${PROJECT_NAME}_tests ${TEST_SOURCES})

# Set include directories for tests
target_include_directories(${PROJECT_NAME}_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/tests
)

# Set Visual Studio debugger working directory for tests
if(MSVC)
    set_target_properties(${PROJECT_NAME}_tests PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    )
endif()

target_link_libraries(${PROJECT_NAME}_tests PRIVATE
    GTest::gtest
    GTest::gtest_main
    sfml-system
    sfml-window
    sfml-graphics
    nlohmann_json::nlohmann_json
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_tests
    DISCOVERY_TIMEOUT 60
    PROPERTIES
        TIMEOUT 60
        ENVIRONMENT "GTEST_COLOR=1"
)

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
