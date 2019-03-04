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

# TODO(!): Update name and srcs
add_library(server_lib src/server.cc src/session.cc src/config_parser.cc
                        src/request_parser.cc src/echo_handler.cc src/mime_types.cc src/static_handler.cc
                        src/error_handler.cc src/status_handler.cc src/create_form_handler.cc src/proxy_handler.cc
                        src/list_meme_handler.cc src/accept_handler.cc src/view_meme_handler src/default_handler.cc
                        src/response.cc src/handler_manager.cc src/response_parser.cc src/client.cc src/types.cc src/utils.cc)

# TODO(!): Update executable name, srcs, and deps
add_executable(server src/server_main.cc)
target_link_libraries(server server_lib Boost::system Boost::log_setup Boost::log Boost::signals)

# TODO(!): Update test executable name, srcs, and deps
add_executable(test_all tests/config_parser_test.cc tests/session_test.cc
                        tests/request_parser_test.cc tests/server_test.cc tests/echo_handler_test.cc tests/static_handler_test.cc
                        tests/mime_types_test.cc tests/utils_test.cc tests/default_handler_test.cc tests/response_test.cc
                        tests/handler_manager_test.cc tests/error_handler_test.cc tests/gmock_server_test.cc
                        tests/response_parser_test.cc tests/client_test.cc tests/proxy_handler_test.cc)
#add_executable(config_parser_test tests/config_parser_test.cc)
#add_executable(session_test tests/session_test.cc)
#add_executable(request_parser_test tests/request_parser_test.cc)
target_link_libraries(test_all server_lib gtest_main Boost::system Boost::log_setup Boost::log Boost::signals gmock)
#target_link_libraries(config_parser_test server_lib gtest_main)
#target_link_libraries(session_test server_lib gtest_main Boost::system)
#target_link_libraries(request_parser_test server_lib gtest_main Boost::system)

# TODO(!): Update with test binary
gtest_discover_tests(test_all WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
#gtest_discover_tests(config_parser_test session_test request_parser_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)

# TODO(!): Update with target/test targets
include(cmake/CodeCoverageReportConfig.cmake)
generate_coverage_report(TARGETS server server_lib TESTS test_all)
#generate_coverage_report(TARGETS server server_lib TESTS config_parser_test session_test request_parser_test)

add_test(NAME IntegrationTest COMMAND intergration_test.sh WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
