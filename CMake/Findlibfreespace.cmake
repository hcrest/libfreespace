## libfreespace - library for communicating with Freespace devices
#
# Copyright 2013-15 Hillcrest Laboratories, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# - Try to find libfreespace
# Once done this will define
#  LIBFREESPACE_FOUND - System has libfreespace
#  LIBFREESPACE_INCLUDE_DIRS - The libfreespace include directories
#  LIBFREESPACE_LIBRARIES - The libraries needed to use libfreespace
#  LIBFREESPACE_DEFINITIONS - Compiler switches required for using libfreespace

if (LIBFREESPACE_LIBRARIES AND LIBFREESPACE_INCLUDE_DIRS)
  # in cache already
  set(LIBFREESPACE_FOUND TRUE)
else (LIBFREESPACE_LIBRARIES AND LIBFREESPACE_INCLUDE_DIRS)
    message(STATUS "CMAKE_FIND_ROOT_PATH = ${CMAKE_FIND_ROOT_PATH}")
    find_path(LIBFREESPACE_INCLUDE_DIR
    NAMES
    freespace/freespace.h
    PATHS
      ${CMAKE_FIND_ROOT_PATH}
      /usr/include
      /usr/local/include
      /opt/local/include
      /sw/include
    PATH_SUFFIXES
      libfreespace
    )

    find_library(LIBFREESPACE_LIBRARY
    NAMES
      freespace
    PATHS
      ${CMAKE_FIND_ROOT_PATH}
      /usr/lib
      /usr/local/lib
      /opt/local/lib
      /sw/lib
    )

    if (LIBFREESPACE_INCLUDE_DIRS AND LIBFREESPACE_LIBRARIES)
     set(LIBFREESPACE_FOUND TRUE)
    endif (LIBFREESPACE_INCLUDE_DIRS AND LIBFREESPACE_LIBRARIES)

    if (LIBFREESPACE_FOUND)
        if (NOT LIBFREESPACE_FIND_QUIETLY)
          message(STATUS "Found libfreespace:")
          message(STATUS " - Includes: ${LIBFREESPACE_INCLUDE_DIRS}")
          message(STATUS " - Libraries: ${LIBFREESPACE_LIBRARIES}")
        endif (NOT LIBFREESPACE_FIND_QUIETLY)
    else (LIBFREESPACE_FOUND)
        if (LIBFREESPACE_FIND_REQUIRED)
          message(FATAL_ERROR "Could not find libusb")
        endif (LIBFREESPACE_FIND_REQUIRED)
    endif (LIBFREESPACE_FOUND)

    # show the LIBFREESPACE_INCLUDE_DIRS and LIBFREESPACE_LIBRARIES variables only in the advanced view
    mark_as_advanced(LIBFREESPACE_INCLUDE_DIRS LIBFREESPACE_LIBRARIES)

endif (LIBFREESPACE_LIBRARIES AND LIBFREESPACE_INCLUDE_DIRS)