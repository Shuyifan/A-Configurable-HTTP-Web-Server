cmake_minimum_required(VERSION 3.10.0)
project(cracking-the-web)

# Make sure developers do not run cmake in the main project directory, to keep
# build artifacts from becoming clutter
if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source builds not allowed.
    Please make a new directory (called a build directory) and run CMake from there.
    You may need to remove CMakeCache.txt." )
endif()

# Turn on debug builds if we are building in a devel shell
if (CMAKE_BUILD_TYPE STREQUAL "" AND "$ENV{DEVEL_SHELL}" STREQUAL "1")
    message(STATUS "Setting debug build type by default in devel shell")
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Output binaries to a sub directory "bin"
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Enable cmake testing
include(CTest)
enable_testing()

# Enable GoogleTest
include(GoogleTest)
add_subdirectory(/usr/src/googletest googletest)


# Enable Boost
# Use static libraries so binaries can be deployed without a full boost install
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.50 REQUIRED COMPONENTS system log signals)
message(STATUS "Boost version: ${Boost_VERSION}")

include_directories(include)

# Update name and srcs
add_library(server_lib src/server.cc src/session.cc src/config_parser.cc src/request_parser.cc
                        src/handler/echo_handler.cc src/handler/error_handler.cc src/handler/static_handler.cc
                        src/handler/status_handler.cc src/handler/create_form_handler.cc src/handler/accept_handler.cc
                        src/handler/view_meme_handler src/handler/list_meme_handler.cc src/handler/delete_handler.cc 
                        src/handler/health_handler.cc src/handler/bad_request_handler.cc src/handler/default_handler.cc
                        src/response.cc src/handler_manager.cc src/utils.cc src/mime_types.cc
                        src/handler/search_handler.cc src/handler/proxy_handler.cc)

# Update executable name, srcs, and deps
add_executable(server src/server_main.cc)
target_link_libraries(server server_lib Boost::system Boost::log_setup Boost::log Boost::signals)

# TODO(!): Update test executable name, srcs, and deps
add_executable(test_all tests/config_parser_test.cc tests/server_test.cc tests/request_parser_test.cc
                        tests/echo_handler_test.cc tests/error_handler_test.cc tests/static_handler_test.cc 
                        tests/create_form_handler_test tests/accept_handler_test.cc tests/view_meme_handler_test.cc 
                        tests/list_meme_handler_test.cc tests/delete_handler_test.cc tests/health_handler_test.cc 
                        tests/bad_request_handler_test.cc tests/default_handler_test.cc tests/handler_manager_test.cc  
                        tests/response_test.cc tests/mime_types_test.cc tests/utils_test.cc tests/proxy_handler_test.cc 
                        tests/status_handler_test.cc tests/search_handler_test.cc)

target_link_libraries(test_all server_lib gtest_main Boost::system Boost::log_setup Boost::log Boost::signals)

# Update with test binary
gtest_discover_tests(test_all WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)

# Update with target/test targets
include(cmake/CodeCoverageReportConfig.cmake)
generate_coverage_report(TARGETS server server_lib TESTS test_all)

add_test(NAME IntegrationTest COMMAND intergration_test.sh WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
