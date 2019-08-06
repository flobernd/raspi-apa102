cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
include(GenerateExportHeader)
include(GNUInstallDirs)

project(RaspiAPA102 VERSION 1.0.0.0 LANGUAGES C CXX)

# =============================================================================================== #
# Overridable options                                                                             #
# =============================================================================================== #

# Build configuration
option(RASPI_APA102_BUILD_SHARED_LIB
    "Build shared library"
    OFF)
option(RASPI_APA102_BUILD_EXAMPLES
    "Build examples"
    OFF)

# =============================================================================================== #
# Exported functions                                                                              #
# =============================================================================================== #



# =============================================================================================== #
# Library configuration                                                                           #
# =============================================================================================== #

if (RASPI_APA102_BUILD_SHARED_LIB)
    add_library("RaspiAPA102" SHARED)
else ()
    add_library("RaspiAPA102" STATIC)
endif ()

set_target_properties("RaspiAPA102" PROPERTIES LINKER_LANGUAGE C)
target_include_directories("RaspiAPA102"
    PUBLIC "include" ${PROJECT_BINARY_DIR}
    PRIVATE "src")
generate_export_header("RaspiAPA102" BASE_NAME "RASPI_APA102" EXPORT_FILE_NAME "RaspiAPA102ExportConfig.h")

target_sources("RaspiAPA102"
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/include/RaspiAPA102/RaspiAPA102.h"
        "src/RaspiAPA102.c")

target_compile_definitions("RaspiAPA102" PRIVATE "_GNU_SOURCE")
find_library(wiringPi_LIB wiringPi REQUIRED)
target_link_libraries("RaspiAPA102" ${wiringPi_LIB})
#find_library(wiringPiSPI_LIB wiringPiSPI REQUIRED)
#target_link_libraries("RaspiAPA102" ${wiringPiSPI_LIB})

# TODO: Install CMake config.
install(TARGETS "RaspiAPA102"
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES
    "${PROJECT_BINARY_DIR}/RaspiAPA102ExportConfig.h"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
install(DIRECTORY "include/" DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# =============================================================================================== #
# Examples                                                                                        #
# =============================================================================================== #

if (RASPI_APA102_BUILD_EXAMPLES)
    add_executable("Fade" "examples/Fade.c")
    target_link_libraries("Fade" "RaspiAPA102")
endif ()

# =============================================================================================== #
