cmake_minimum_required(VERSION 3.15)

set(TBOX_ALARM_VERSION_MAJOR 0)
set(TBOX_ALARM_VERSION_MINOR 0)
set(TBOX_ALARM_VERSION_PATCH 1)
set(TBOX_ALARM_VERSION ${TBOX_ALARM_VERSION_MAJOR}.${TBOX_ALARM_VERSION_MINOR}.${TBOX_ALARM_VERSION_PATCH})

add_definitions(-DLOG_MODULE_ID="tbox.alarm")

set(TBOX_LIBRARY_NAME tbox_alarm)

set(TBOX_ALARM_HEADERS
    alarm.h
    weekly_alarm.h
    oneshot_alarm.h
    cron_alarm.h
    workday_alarm.h
    workday_calendar.h)

set(TBOX_ALARM_SOURCES
    alarm.cpp
    weekly_alarm.cpp
    oneshot_alarm.cpp
    cron_alarm.cpp
    3rd-party/ccronexpr.cpp
    workday_alarm.cpp
    workday_calendar.cpp)

set(TBOX_ALARM_TEST_SOURCES
    workday_calendar_test.cpp)

add_library(${TBOX_LIBRARY_NAME} ${TBOX_BUILD_LIB_TYPE} ${TBOX_ALARM_SOURCES})
add_library(tbox::${TBOX_LIBRARY_NAME} ALIAS ${TBOX_LIBRARY_NAME})

set_target_properties(
    ${TBOX_LIBRARY_NAME} PROPERTIES
    VERSION ${TBOX_ALARM_VERSION}
    SOVERSION ${TBOX_ALARM_VERSION_MAJOR}
)

if(${TBOX_ENABLE_TEST})
    add_executable(${TBOX_LIBRARY_NAME}_test ${TBOX_ALARM_TEST_SOURCES})

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
    FILES ${TBOX_ALARM_HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/tbox/alarm
)


# generate and install export file
install(
    EXPORT ${TBOX_LIBRARY_NAME}_targets
    FILE ${TBOX_LIBRARY_NAME}_targets.cmake
    NAMESPACE tbox::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/tbox
)
