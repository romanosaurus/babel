project(Babel)
cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)
if (UNIX)
    add_definitions("-fPIC -Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wuninitialized -Wunreachable-code")
endif (UNIX)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(KEEP_RPATHS)

if (APPLE)
    set(CMAKE_INSTALL_RPATH "@executable_path/../lib")
endif (APPLE)

file(WRITE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/qt.conf [Paths]\nPrefix=${CONAN_QT_ROOT})

find_package(Qt5 COMPONENTS Widgets Network REQUIRED)

file(
        GLOB_RECURSE
        BABEL_SOURCES_CLIENT
        ${PROJECT_SOURCE_DIR}/client/sources/*.cpp
        ${PROJECT_SOURCE_DIR}/client/sources/*.hpp
)

file(
        GLOB_RECURSE
        BABEL_SOURCES_SERVER
        ${PROJECT_SOURCE_DIR}/server/sources/*.cpp
        ${PROJECT_SOURCE_DIR}/server/sources/*.hpp
)

file(
        GLOB_RECURSE
        BABEL_SOURCES_COMMON
        ${PROJECT_SOURCE_DIR}/commons/*.cpp
        ${PROJECT_SOURCE_DIR}/commons/*.hpp
)

file(
        GLOB_RECURSE
        BABEL_SOURCES_TESTS
        ${PROJECT_SOURCE_DIR}/client/sources/*.cpp
        ${PROJECT_SOURCE_DIR}/client/sources/*.hpp
        ${PROJECT_SOURCE_DIR}/server/sources/*.cpp
        ${PROJECT_SOURCE_DIR}/server/sources/*.hpp
        ${PROJECT_SOURCE_DIR}/commons/*.cpp
        ${PROJECT_SOURCE_DIR}/commons/*.hpp
        ${PROJECT_SOURCE_DIR}/tests/*.cpp
        ${PROJECT_SOURCE_DIR}/tests/*.hpp
)
list(REMOVE_ITEM BABEL_SOURCES_TESTS "${PROJECT_SOURCE_DIR}/server/sources/main.cpp")
list(REMOVE_ITEM BABEL_SOURCES_TESTS "${PROJECT_SOURCE_DIR}/client/sources/main.cpp")

add_executable(babel_client ${BABEL_SOURCES_CLIENT} ${BABEL_SOURCES_COMMON})
target_include_directories(
        babel_client PUBLIC
        ${CMAKE_INCLUDE_PATH}
        ${PROJECT_SOURCE_DIR}/commons
        ${PROJECT_SOURCE_DIR}/client/sources
)
target_link_libraries(
        babel_client
        ${CONAN_LIBS}
        Qt5::Widgets
        Qt5::Network
)

add_executable(babel_server ${BABEL_SOURCES_SERVER} ${BABEL_SOURCES_COMMON})
target_include_directories(
        babel_server PUBLIC
        ${CMAKE_INCLUDE_PATH}
        ${PROJECT_SOURCE_DIR}/commons
        ${PROJECT_SOURCE_DIR}/server/sources
)
target_link_libraries(
        babel_server
        ${CONAN_LIBS}
)

if (UNIX)
    add_executable(tests ${BABEL_SOURCES_TESTS})
    target_include_directories(
            tests PUBLIC
            ${CMAKE_INCLUDE_PATH}
            ${PROJECT_SOURCE_DIR}/commons
            ${PROJECT_SOURCE_DIR}/client/sources
            ${PROJECT_SOURCE_DIR}/server/sources
    )
    target_link_libraries(
            tests
            criterion
            ${CONAN_LIBS}
            Qt5::Widgets
            Qt5::Network
    )
    add_custom_target(tests_run COMMAND tests)
endif (UNIX)

file(GLOB BUILD_DEPENDENCY "build_dependency/*")

file(COPY ${BUILD_DEPENDENCY} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

