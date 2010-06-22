# This file is part of libfreespace.
#  
# Copyright (c) 2009 Hillcrest Laboratories, Inc. 
# 
# libfreespace is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
# 
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
# 
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

lib_LTLIBRARIES = libfreespace.la

LINUX_HOTPLUG_SRC = linux_hotplug.c
DARWIN_HOTPLUG_SRC = darwin_hotplug.c

EXTRA_DIST = $(LINUX_HOTPLUG_SRC) $(DARWIN_HOTPLUG_SRC) 

if OS_LINUX
HOTPLUG_SRCS = $(LINUX_HOTPLUG_SRC)
endif

if OS_DARWIN
HOTPLUG_SRCS = $(DARWIN_HOTPLUG_SRC)
endif

libfreespace_la_SOURCES = ../common/freespace_codecs.c ../common/freespace_printers.c ../common/freespace_deviceTable.c freespace.c hotplug.h $(HOTPLUG_SRCS)
include_HEADERS = ../include/freespace/freespace_codecs.h ../include/freespace/freespace_printers.h ../include/freespace/freespace.h ../include/freespace/freespace_common.h ../include/freespace/freespace_deviceTable.h

includedir = $(prefix)/include/freespace

BUILT_SOURCES = ../common/freespace_printers.c ../common/freespace_codecs.c ../include/freespace/freespace_printers.h ../include/freespace/freespace_codecs.h

../common/freespace_codecs.c: ../common/freespace_printers.c
../common/freespace_printers.c: ../common/messageCodeGenerator.py ../common/setupMessages.py
	../common/messageCodeGenerator.py ../common/setupMessages.py

AM_CFLAGS = -Wall -I../include -lm
