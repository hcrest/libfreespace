cmake_minimum_required (VERSION 2.6)

include(FindDoxygen)
if (NOT DOXYGEN)
    message(STATUS "Doxygen not found - Reference manual cannot be created")
    return()
ENDIF ()

set(LIBFREESPACE_DOCS_ENABLED ON CACHE BOOL "Enable doxygen docs as a build target")
if (NOT LIBFREESPACE_DOCS_ENABLED)
	return()
endif()
set(LIBFREESPACE_DOCS_INTERNAL OFF CACHE BOOL "Generate doxygen for src files (in addition to API)")

set(DOXYGEN_SRC "${PROJECT_BINARY_DIR}/doc/mainpage.h" "${PROJECT_SOURCE_DIR}/include/" "${PROJECT_BINARY_DIR}/include/")

if (LIBFREESPACE_DOCS_INTERNAL)
	set(DOXYGEN_SRC ${DOXYGEN_SRC} "${PROJECT_SOURCE_DIR}/linux/" "${PROJECT_SOURCE_DIR}/win32/")
endif()


# convert to a space seperated string
string(REPLACE ";" " " DOXYGEN_SRC "${DOXYGEN_SRC}")


configure_file("${PROJECT_SOURCE_DIR}/doc/Doxyfile.in" "${PROJECT_BINARY_DIR}/doc/Doxyfile")
configure_file("${PROJECT_SOURCE_DIR}/doc/mainpage.h.in" "${PROJECT_BINARY_DIR}/doc/mainpage.h")

add_custom_target(libfreespace-docs
              ${DOXYGEN}
              DEPENDS "${PROJECT_SOURCE_DIR}/doc/Doxyfile.in" "${PROJECT_SOURCE_DIR}/doc/mainpage.h.in"
          	  WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/doc/")

