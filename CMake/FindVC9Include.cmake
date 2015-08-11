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
#  Find VC9Include
#  Returns VC9_INCLUDE for VS2008

IF ("${VC9_INCLUDE}" STREQUAL "" OR "${VC9_INCLUDE}" STREQUAL "VC9_INCLUDE-NOTFOUND")
	IF (NOT $ENV{VS90COMNTOOLS} STREQUAL "")
		# First try finding it using COMNTOOLS
		get_filename_component(VC9_INCLUDE_SEARCH_DIRS $ENV{VS90COMNTOOLS} PATH)
		get_filename_component(VC9_INCLUDE_SEARCH_DIRS ${VC9_INCLUDE_SEARCH_DIRS} PATH)
		SET(VC9_INCLUDE_SEARCH_DIRS "${VC9_INCLUDE_SEARCH_DIRS}/VC/include")
	ENDIF()
	IF (EXISTS "C:/Program Files (x86)/Microsoft Visual Studio 9.0/VC/include")
		# Next add a raw guess path
		SET(VC9_INCLUDE_SEARCH_DIRS "C:/Program Files (x86)/Microsoft Visual Studio 9.0/VC/include" ${VC9_INCLUDE_SEARCH_DIRS})
	ENDIF()
	FIND_PATH(VC9_INCLUDE NAMES "string.h" PATHS ${VC9_INCLUDE_SEARCH_DIRS})
	SET(VC9_INCLUDE "${VC9_INCLUDE}" CACHE PATH "Path to the Visual Studio 9.0 Include directory")
	IF ("${VC9_INCLUDE}" STREQUAL "" OR "${VC9_INCLUDE}" STREQUAL "VC9_INCLUDE-NOTFOUND")
		# If neither of those worked prompt user
		MESSAGE(FATAL_ERROR "Visual Studio 9.0 Include Path not found - please set the VC9_INCLUDE path.")
	ENDIF()
ENDIF()
IF (NOT EXISTS "${VC9_INCLUDE}/string.h")
	# Checks for string.h to see if it makes sense as an include path
	MESSAGE(FATAL_ERROR "VC9_INCLUDE does not point to a valid Include path. Look for Microsoft Visual Studio 9.0/VC/include/")
ENDIF()