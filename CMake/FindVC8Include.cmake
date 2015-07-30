## libfreespace - library for communicating with Freespace devices
#
# Copyright 2015 Hillcrest Labs, Inc.
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
#  Find VC8Include
#  Returns VC8_INCLUDE for VS2005

IF ("${VC8_INCLUDE}" STREQUAL "" OR "${VC8_INCLUDE}" STREQUAL "VC8_INCLUDE-NOTFOUND")
	IF (NOT $ENV{VS90COMNTOOLS} STREQUAL "")
		# First try finding it using COMNTOOLS
		get_filename_component(VC8_INCLUDE_SEARCH_DIRS $ENV{VS80COMNTOOLS} PATH)
		get_filename_component(VC8_INCLUDE_SEARCH_DIRS ${VC8_INCLUDE_SEARCH_DIRS} PATH)
		SET(VC8_INCLUDE_SEARCH_DIRS "${VC8_INCLUDE_SEARCH_DIRS}/VC/include")
	ENDIF()
	IF (EXISTS "C:/Program Files (x86)/Microsoft Visual Studio 8.0/VC/include")
		# Next add a raw guess path
		SET(VC8_INCLUDE_SEARCH_DIRS "C:/Program Files (x86)/Microsoft Visual Studio 8.0/VC/include" ${VC8_INCLUDE_SEARCH_DIRS})
	ENDIF()
	FIND_PATH(VC8_INCLUDE NAMES "string.h" PATHS ${VC8_INCLUDE_SEARCH_DIRS})
	SET(VC8_INCLUDE "${VC8_INCLUDE}" CACHE PATH "Path to the Visual Studio 8.0 Include directory")
	IF ("${VC8_INCLUDE}" STREQUAL "" OR "${VC8_INCLUDE}" STREQUAL "VC8_INCLUDE-NOTFOUND")
		# If neither of those worked prompt user
		MESSAGE(FATAL_ERROR "Visual Studio 8.0 Include Path not found - please set the VC8_INCLUDE path.")
	ENDIF()
ENDIF()
IF (NOT EXISTS "${VC8_INCLUDE}/string.h")
	# Checks for string.h to see if it makes sense as an include path
	MESSAGE(FATAL_ERROR "VC8_INCLUDE does not point to a valid Include path. Look for Microsoft Visual Studio 8.0/VC/include/")
ENDIF()