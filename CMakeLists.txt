cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

if(NOT ${CMAKE_VERSION} VERSION_LESS "3.1")
  cmake_policy(SET CMP0054 NEW)
endif()

MESSAGE( STATUS ">>> Build type set to " ${CMAKE_BUILD_TYPE} " <<<" )

project(IQFeedClient)
set(PROJECT_LONGNAME "IQFeedClient")
set(PROJECT_VERSION "1.0.0")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W2")
    set(QT_DIR C:/Qt/Qt5.5.0/5.5/msvc2013_64)
    set(CMAKE_PREFIX_PATH ${QT_DIR})

    add_definitions(-DWIN64 -DQT_DLL -DQT_DEBUG -DNOMINMAX)
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Source)
set(HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Include)

set(MAIN_SOURCES ${SOURCE_DIR}/connection.cpp
${SOURCE_DIR}/iqConnectConnection.cpp
${SOURCE_DIR}/iqFeedClient.cpp
${SOURCE_DIR}/main.cpp
${SOURCE_DIR}/processManager.cpp
${SOURCE_DIR}/protectedBuffer.cpp
${SOURCE_DIR}/socketThread.cpp)

set(SOURCES ${MAIN_SOURCES})

set(MAIN_HEADERS ${HEADER_DIR}/connection.h
${HEADER_DIR}/iqConnectConnection.h
${HEADER_DIR}/iqFeedClient.h
${HEADER_DIR}/processManager.h
${HEADER_DIR}/protectedBuffer.h
${HEADER_DIR}/socketThread.h)
 
set(HEADERS ${MAIN_HEADERS})

qt5_wrap_cpp(MOC_FILES ${HEADERS})

set(INCLUDE_DIRS ${SOURCE_DIR}
                 ${HEADER_DIR})

include_directories(${INCLUDE_DIRS})

add_executable(${PROJECT_LONGNAME} ${SOURCES} ${HEADERS} ${MOC_FILES})

qt5_use_modules(${PROJECT_LONGNAME} Core Network)
