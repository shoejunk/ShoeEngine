cmake_minimum_required(VERSION 3.14)

# Project declaration
project(ShoeEngine VERSION 0.1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Download CPM.cmake ---
set(CPM_DOWNLOAD_VERSION 0.38.6)
set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
if(NOT EXISTS ${CPM_DOWNLOAD_LOCATION})
    message(STATUS "Downloading CPM.cmake...")
    file(DOWNLOAD
        https://github.com/cpm-cmake/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake
        ${CPM_DOWNLOAD_LOCATION}
    )
endif()
include(${CPM_DOWNLOAD_LOCATION})

# --- Dependencies ---

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

# Add GoogleTest dependency (for tests)
CPMAddPackage(
    NAME GoogleTest
    GITHUB_REPOSITORY google/googletest
    GIT_TAG v1.14.0
    VERSION 1.14.0
    OPTIONS
        "INSTALL_GTEST OFF"
        "gtest_force_shared_crt ON"
)

# --- Core Engine Library Setup ---

# Collect all source and header files from src/
file(GLOB_RECURSE ALL_SRC "src/*.cpp")
file(GLOB_RECURSE ALL_HEADERS "src/*.h")

# Exclude main.cpp (which contains the main() function) from the library build.
list(FILTER ALL_SRC EXCLUDE REGEX ".*main\\.cpp$")

set(LIB_SOURCES ${ALL_SRC})
set(LIB_HEADERS ${ALL_HEADERS})

# Create the engine library (core code) as a static library
add_library(${PROJECT_NAME}_lib STATIC ${LIB_SOURCES} ${LIB_HEADERS})

# Include our own headers (from src/)
target_include_directories(${PROJECT_NAME}_lib PUBLIC ${CMAKE_SOURCE_DIR}/src)

# Link third-party dependencies so that their include directories are available.
# Using PUBLIC ensures that any target linking ${PROJECT_NAME}_lib will also see these include paths.
target_link_libraries(${PROJECT_NAME}_lib PUBLIC
    sfml-system
    sfml-window
    sfml-graphics
    sfml-audio
    sfml-network
    nlohmann_json::nlohmann_json
)

# Visual Studio: group files in their native directory structure (headers and cpp files together)
if(MSVC)
    source_group(TREE ${CMAKE_SOURCE_DIR}/src FILES ${LIB_SOURCES} ${LIB_HEADERS})
endif()

# --- Main Executable Setup ---

# Define the main file separately (assumed to be src/main.cpp)
set(MAIN_FILE "${CMAKE_SOURCE_DIR}/src/main.cpp")

add_executable(${PROJECT_NAME} ${MAIN_FILE})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Link the core library (which propagates SFML and nlohmann/json include paths) to the executable.
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_lib)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

if(MSVC)
    source_group(TREE ${CMAKE_SOURCE_DIR}/src FILES ${MAIN_FILE})
    set_target_properties(${PROJECT_NAME} PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    )
endif()

# Copy SFML DLLs for the main executable
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:sfml-system>
        $<TARGET_FILE:sfml-window>
        $<TARGET_FILE:sfml-graphics>
        $<TARGET_FILE:sfml-audio>
        $<TARGET_FILE:sfml-network>
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

# Copy the data directory and sample.json to the main executable output
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/data
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/data
)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/data/sample.json
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/data/sample.json
)

# --- Test Executable Setup ---

enable_testing()

# Collect test source and header files from tests/
file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")
file(GLOB_RECURSE TEST_HEADERS "tests/*.h")

if(MSVC)
    source_group(TREE ${CMAKE_SOURCE_DIR}/tests FILES ${TEST_SOURCES} ${TEST_HEADERS})
endif()

# Create the test executable; tests are built only from the tests/ folder.
add_executable(${PROJECT_NAME}_tests ${TEST_SOURCES} ${TEST_HEADERS})
target_include_directories(${PROJECT_NAME}_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/tests
    ${CMAKE_SOURCE_DIR}/src  # Allow tests to include core engine headers.
)
target_link_libraries(${PROJECT_NAME}_tests PRIVATE
    ${PROJECT_NAME}_lib
    GTest::gtest
    GTest::gtest_main
)

if(MSVC)
    set_target_properties(${PROJECT_NAME}_tests PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    )
endif()

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_tests
    DISCOVERY_TIMEOUT 60
    PROPERTIES
        TIMEOUT 60
        ENVIRONMENT "GTEST_COLOR=1"
)

set_target_properties(${PROJECT_NAME}_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Copy SFML DLLs for the test executable
add_custom_command(TARGET ${PROJECT_NAME}_tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:sfml-system>
        $<TARGET_FILE:sfml-window>
        $<TARGET_FILE:sfml-graphics>
        $<TARGET_FILE:sfml-audio>
        $<TARGET_FILE:sfml-network>
        $<TARGET_FILE_DIR:${PROJECT_NAME}_tests>
)
