cmake_minimum_required(VERSION 3.20)

project(utf42 LANGUAGES CXX)

include(GNUInstallDirs)
include(FetchContent)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(UTF42_WITH_UTFCPP "Build examples/tests with utf8cpp" OFF)
option(UTF42_WITH_DOXYGEN "Build documentation with awesome doxygen" OFF)

message(STATUS "Use utfcpp: ${UTF42_WITH_UTFCPP}")
message(STATUS "Use doxygen: ${UTF42_WITH_DOXYGEN}")



# ------------------------------------------------------------
# Header-only library
# ------------------------------------------------------------
add_library(utf42 INTERFACE)

target_include_directories(utf42 INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/utf42>
)

add_library(utf42::utf42 ALIAS utf42)

# ------------------------------------------------------------
# Optional utf8cpp (examples/tests only)
# ------------------------------------------------------------

if (UTF42_WITH_UTFCPP)
    FetchContent_Declare(
            utfcpp
            GIT_REPOSITORY https://github.com/nemtrif/utfcpp.git
            GIT_TAG master
    )
    FetchContent_MakeAvailable(utfcpp)

    # Add utf8cpp library as an alias for easier linking
    # add_library(utf8cpp::utf8cpp INTERFACE IMPORTED)
endif ()

# ------------------------------------------------------------
# Example
# ------------------------------------------------------------
add_executable(example_utf42 main.cpp)
target_link_libraries(example_utf42 PRIVATE utf42)

if (UTF42_WITH_UTFCPP)
    message(STATUS ${UTF42_WITH_UTFCPP})
    target_link_libraries(example_utf42 PRIVATE utf8cpp)
endif ()

# ------------------------------------------------------------
# Tests
# ------------------------------------------------------------
add_executable(test_utf42 test.cpp)
target_link_libraries(test_utf42 PRIVATE utf42)

if (UTF42_WITH_UTFCPP)
    target_link_libraries(test_utf42 PRIVATE utf8cpp)
endif ()

# ------------------------------------------------------------
# Installation
# ------------------------------------------------------------
install(TARGETS utf42
        EXPORT utf42Targets)

install(FILES utf42.h
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/utf42)

install(EXPORT utf42Targets
        NAMESPACE utf42::
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/utf42/cmake)

# ------------------------------------------------------------
# Documentation
# ------------------------------------------------------------

if (UTF42_WITH_DOXYGEN)

    # Doxygen configuration
    find_package(Doxygen REQUIRED)
    message(STATUS "Doxygen executable: ${DOXYGEN}")

    #Doxygen Awesome
    FetchContent_Declare(
            doxygen-awesome-css
            URL https://github.com/jothepro/doxygen-awesome-css/archive/refs/heads/main.zip
            DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
    FetchContent_MakeAvailable(doxygen-awesome-css)
    FetchContent_GetProperties(doxygen-awesome-css SOURCE_DIR AWESOME_CSS_DIR)

    # Generate the Doxyfile
    set(PROJECT_DIR ${CMAKE_CURRENT_LIST_DIR})
    set(DOXYFILE_IN ${CMAKE_CURRENT_LIST_DIR}/Doxyfile)
    set(DOXYFILE_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)

    add_custom_target(doc
            COMMAND ${DOXYGEN} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM
    )

endif ()