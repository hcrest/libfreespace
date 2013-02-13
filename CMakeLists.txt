# 
# This file is part of libfreespace.
# Copyright (c) 2009-2013 Hillcrest Laboratories, Inc.
# libfreespace is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
# 
#

cmake_minimum_required (VERSION 2.6)
project (freespace CXX C)
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${PROJECT_SOURCE_DIR}/CMake")

set (PROJECT_VERSION_MAJOR 0)
set (PROJECT_VERSION_MINOR 7)
set (PROJECT_VERSION_PATCH 0)

set (PROJECT_VERSIONSTR "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")

include(FindPythonInterp)
if (NOT DEFINED PYTHONINTERP_FOUND)
	message(FATAL_ERROR "Could not locate python interpreter, python is required to generate freespace messages)")
endif()


### Project Configuration Options
set(LIBFREESPACE_ADDITIONAL_MESSAGE_FILE "" CACHE STRING "An additional HID message definition file")
set(LIBFREESPACE_CODECS_ONLY OFF CACHE BOOL "Build only the libfreespace codecs")
set(LIBFREESPACE_LIB_TYPE "SHARED" CACHE STRING "An additional HID message definition file, set to SHARED or STATIC")
set(LIBFREESPACE_BACKEND "" CACHE STRING "Specify an alternate backend on some paltforms. On Linux, valid values are 'hidraw' and 'libusb'")
set(LIBFREESPACE_HIDRAW_THREADED_WRITES OFF CACHE STRING "Enable writes in a backend thread when using hidraw")

set(src_gen
    "${PROJECT_BINARY_DIR}/gen_src/freespace_codecs.c"
    "${PROJECT_BINARY_DIR}/gen_src/freespace_printers.c"
)

set(hdr_gen
    "${PROJECT_BINARY_DIR}/include/freespace/freespace_codecs.h"
    "${PROJECT_BINARY_DIR}/include/freespace/freespace_printers.h"
)

### Message Code Generator #######################

# Build rule to generate the HCOMM messages as needed.
add_custom_command(
    OUTPUT ${src_gen} ${hdr_gen}
    COMMAND
        ${PYTHON_EXECUTABLE}        
        "${PROJECT_SOURCE_DIR}/common/messageCodeGenerator.py"
        "-I" "${PROJECT_BINARY_DIR}/include/"
        "-s" "${PROJECT_BINARY_DIR}/gen_src/"
        "${PROJECT_SOURCE_DIR}/common/setupMessages.py"
        "${LIBFREESPACE_ADDITIONAL_MESSAGE_FILE}"
    ${buildMessageCommand}
    DEPENDS 
        "common/messageCodeGenerator.py"
        "common/setupMessages.py"
        "${LIBFREESPACE_ADDITIONAL_MESSAGE_FILE}"
)


include_directories("include")
include_directories("${PROJECT_BINARY_DIR}/include")


# Determine the target endianness and set the libfreespace flag.
if(ANDROID) 
    #TEST_BIG_ENDIAN doesn't work with Android NDK
    add_definitions( -DFREESPACE_LITTLE_ENDIAN )
    SET( CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -lm" )
else()
    include(TestBigEndian NO_POLICY_SCOPE)
    TEST_BIG_ENDIAN(isBigEndian)
    if (isBigEndian)
        add_definitions( -DFREESPACE_BIG_ENDIAN )
    else()
        add_definitions( -DFREESPACE_LITTLE_ENDIAN )
    endif(isBigEndian)
endif()

# List the common source files
set(LIBFREESPACE_CODEC_SRCS "${src_gen}")
set (LIBFREESPACE_COMMON_SRCS 
    "common/freespace_deviceTable.c"
    "${LIBFREESPACE_CODEC_SRCS}"
)

# TODO : reconcile config-win32.h & config.h
configure_file(${PROJECT_SOURCE_DIR}/config.h.in ${PROJECT_BINARY_DIR}/include/config.h)


if (LIBFREESPACE_CODECS_ONLY)
    # BUILD WITHOUT HID DEPENDENCIES
    message(STATUS "Building libfreespace without HID support")
    if (ANDROID)
        add_library(freespace-codecs 
            STATIC
            ${LIBFREESPACE_CODEC_SRCS}
        )
    else()
        add_library(freespace-codecs 
            ${LIBFREESPACE_LIB_TYPE}
            ${LIBFREESPACE_CODEC_SRCS}
        )
    endif()
elseif (WIN32)
    # WINDOWS BUILD CONFIGURATION
    set (WINDDK "${PROJECT_SOURCE_DIR}/../WinDDK/7600.16385.1")
    message( STATUS "WinDDK in ${WINDDK}")
    include_directories("${WINDDK}/inc")
    set (WINDDK_LIB "${WINDDK}/lib/win7/i386")
    add_definitions(-DUNICODE -D_UNICODE)
    
    add_library(freespace ${LIBFREESPACE_LIB_TYPE}
        ${LIBFREESPACE_COMMON_SRCS}
        "win32/freespace_device.c"
        "win32/freespace_deviceMgr.c"
        "win32/freespace_discovery.c"
        "win32/freespace_discoveryDetail.c"
    )
    
    target_link_libraries(freespace "${WINDDK_LIB}/hid.lib")
    target_link_libraries(freespace setupapi.lib)

elseif(UNIX)
    # Additional headers
    include(CheckIncludeFiles)
    check_include_files(sys/time.h HAVE_SYS_TIME_H)
    if (NOT HAVE_SYS_TIME_H)
        message(FATAL_ERROR "Could not find include file <sys/time.h>")
    endif()
    if (LIBFREESPACE_BACKEND STREQUAL "hidraw")
        check_include_files(linux/hidraw.h HAVE_LINUX_HIDRAW_H)
        if (NOT HAVE_LINUX_HIDRAW_H)
            message(FATAL_ERROR "Could not find include file <linux/hidraw.h>")
        endif()

        if (LIBFREESPACE_HIDRAW_THREADED_WRITES) 
            add_definitions(-DLIBFRESPACE_THREADED_WRITES -pthread)
            list(APPEND CMAKE_EXE_LINKER_FLAGS -pthread)
        endif()
            
        add_library(freespace ${LIBFREESPACE_LIB_TYPE}
            ${LIBFREESPACE_COMMON_SRCS}
            "linux/freespace_hidraw.c"
            "linux/linux_hotplug.c"
         )
         
    elseif (LIBFREESPACE_BACKEND STREQUAL "libusb" OR LIBFREESPACE_BACKEND STREQUAL "")
        #set(libusb_1_FIND_QUIETLY ON)
        set(LIBUSB1_FIND_REQUIRED ON)
        find_package(libusb-1.0)
        
        list(APPEND CMAKE_EXE_LINKER_FLAGS -pthread)
        add_definitions(-pthread)    
           
        include_directories(${LIBUSB_1_INCLUDE_DIRS})

        add_library(freespace ${LIBFREESPACE_LIB_TYPE}
            ${LIBFREESPACE_COMMON_SRCS}
            "linux/freespace.c"
            "linux/linux_hotplug.c"
         )

        target_link_libraries(freespace ${LIBUSB_1_LIBRARIES})
    else()
        message(FATAL_ERROR "Unsupported backened -- ${LIBFREESPACE_BACKEND}")
    endif()
else(APPLE)
    # Mac OSX / Darwing build configuration
    add_library(freespace ${LIBFREESPACE_LIB_TYPE}
        ${LIBFREESPACE_COMMON}
        "linux/freespace.c"
        "linux/darwin_hotplug.c"
    )
endif()


install(TARGETS freespace LIBRARY DESTINATION lib)
install(DIRECTORY include/freespace DESTINATION include)
install(FILES ${hdr_gen} DESTINATION include/freespace)

SET(CPACK_GENERATOR TGZ)
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "CMake is a build tool")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "libfreespace")
SET(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "libfreespace")
SET(CPACK_PACKAGE_NAME "libfreespace")
SET(CPACK_PACKAGE_VENDOR "Hillcrest Labs, Inc.")
SET(CPACK_PACKAGE_VERSION "${PROJECT_VERSIONSTR}")
SET(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
SET(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
SET(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
SET(CPACK_SOURCE_GENERATOR ZIP)
#SET(CPACK_SOURCE_STRIP_FILES "")
SET(CPACK_STRIP_FILES "libfreespace")

#SET(CPACK_NSIS_DISPLAY_NAME "libfreespace ${PROJECT_VERSIONSTR}")
#SET(CPACK_INSTALL_CMAKE_PROJECTS "/home/andy/vtk/CMake-bin;CMake;ALL;/")
#SET(CPACK_OUTPUT_CONFIG_FILE "/home/andy/vtk/CMake-bin/CPackConfig.cmake")
#SET(CPACK_PACKAGE_DESCRIPTION_FILE "README")

#SET(CPACK_RESOURCE_FILE_LICENSE "/home/andy/vtk/CMake/Copyright.txt")
#SET(CPACK_RESOURCE_FILE_README "/home/andy/vtk/CMake/Templates/CPack.GenericDescription.txt")
#SET(CPACK_RESOURCE_FILE_WELCOME "/home/andy/vtk/CMake/Templates/CPack.GenericWelcome.txt")

#SET(CPACK_SOURCE_OUTPUT_CONFIG_FILE "/home/andy/vtk/CMake-bin/CPackSourceConfig.cmake")

#SET(CPACK_SYSTEM_NAME "Linux-i686")
#SET(CPACK_TOPLEVEL_TAG "Linux-i686")


include(CPack)
