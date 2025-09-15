cmake_minimum_required(VERSION 3.30)
project(
  stockholm
  VERSION 0.0.1
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include(GNUInstallDirs)

# Create target
add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

# Set include directories
target_include_directories(
  ${PROJECT_NAME}
  INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

# Find and link dependencies
find_package(fmt CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} INTERFACE fmt::fmt-header-only)

# Set C++ standard requirement
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_20)

# Optional: Build tests
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR OR STOCKHOLM_BUILD_TESTS)
  option(STOCKHOLM_BUILD_TESTS "Build stockholm tests" ON)
else()
  option(STOCKHOLM_BUILD_TESTS "Build stockholm tests" OFF)
endif()

if(STOCKHOLM_BUILD_TESTS)
  find_package(Catch2 CONFIG REQUIRED)
  add_executable(
    stockholm_test
    test/unit/test_pattern_basic.cpp test/unit/test_pattern_quantifiers.cpp
    test/unit/test_pattern_classes.cpp test/unit/test_pattern_operators.cpp
    test/unit/test_yarn.cpp test/unit/test_map.cpp)

  target_link_libraries(stockholm_test PRIVATE Catch2::Catch2WithMain
                                               ${PROJECT_NAME}::${PROJECT_NAME})

  include(CTest)
  include(Catch)
  enable_testing()
  catch_discover_tests(stockholm_test)
endif()

# Installation
install(
  TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}Targets
  INCLUDES
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Install headers
install(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  FILES_MATCHING
  PATTERN "*.hpp"
  PATTERN "*.h")

# Generate and install CMake config files
include(CMakePackageConfigHelpers)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion)

configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

install(
  EXPORT ${PROJECT_NAME}Targets
  FILE ${PROJECT_NAME}Targets.cmake
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
              "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)
