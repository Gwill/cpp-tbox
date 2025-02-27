cmake_minimum_required(VERSION 3.15)

set(TBOX_UTIL_VERSION_MAJOR 0)
set(TBOX_UTIL_VERSION_MINOR 0)
set(TBOX_UTIL_VERSION_PATCH 2)
set(TBOX_UTIL_VERSION ${TBOX_UTIL_VERSION_MAJOR}.${TBOX_UTIL_VERSION_MINOR}.${TBOX_UTIL_VERSION_PATCH})

add_definitions(-DLOG_MODULE_ID="tbox.util")

set(TBOX_LIBRARY_NAME tbox_util)

set(TBOX_UTIL_HEADERS
    pid_file.h
    string.h
    fs.h
    argument_parser.h
    split_cmdline.h
    serializer.h
    time_counter.h
    async_pipe.h
    timestamp.h
    json.h
    base64.h
    checksum.h
    crc.h
    json_deep_loader.h
    execute_cmd.h)

set(TBOX_UTIL_SOURCES
    pid_file.cpp
    string.cpp
    fs.cpp
    argument_parser.cpp
    split_cmdline.cpp
    serializer.cpp
    time_counter.cpp
    async_pipe.cpp
    timestamp.cpp
    json.cpp
    base64.cpp
    checksum.cpp
    crc.cpp
    json_deep_loader.cpp
    execute_cmd.cpp)

set(TBOX_UTIL_TEST_SOURCES
    pid_file_test.cpp
    string_test.cpp
    fs_test.cpp
    argument_parser_test.cpp
    split_cmdline_test.cpp
    serializer_test.cpp
    time_counter_test.cpp
    async_pipe_test.cpp
    json_test.cpp
    base64_test.cpp
    checksum_test.cpp
    crc_test.cpp
    execute_cmd_test.cpp)

add_library(${TBOX_LIBRARY_NAME} ${TBOX_BUILD_LIB_TYPE} ${TBOX_UTIL_SOURCES})
add_library(tbox::${TBOX_LIBRARY_NAME} ALIAS ${TBOX_LIBRARY_NAME})

set_target_properties(
    ${TBOX_LIBRARY_NAME} PROPERTIES
    VERSION ${TBOX_UTIL_VERSION}
    SOVERSION ${TBOX_UTIL_VERSION_MAJOR}
)

if(${TBOX_ENABLE_TEST})
    add_executable(${TBOX_LIBRARY_NAME}_test ${TBOX_UTIL_TEST_SOURCES})
    target_link_libraries(${TBOX_LIBRARY_NAME}_test gmock_main gmock gtest pthread ${TBOX_LIBRARY_NAME} tbox_base rt dl)
    add_test(NAME ${TBOX_LIBRARY_NAME}_test COMMAND ${TBOX_LIBRARY_NAME}_test)
endif()

# install the target and create export-set
install(
    TARGETS ${TBOX_LIBRARY_NAME}
    EXPORT ${TBOX_LIBRARY_NAME}_targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# install header file
install(
    FILES ${TBOX_UTIL_HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/tbox/base
)


# generate and install export file
install(
    EXPORT ${TBOX_LIBRARY_NAME}_targets
    FILE ${TBOX_LIBRARY_NAME}_targets.cmake
    NAMESPACE tbox::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/tbox
)
