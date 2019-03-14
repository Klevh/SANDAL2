# Locate SANDAL2 library
#
# This module defines:
#
# ::
#
#   SANDAL2_LIBRARIES, the name of the library to link against
#   SANDAL2_INCLUDE_DIRS, where to find the headers
#   SANDAL2_FOUND, if false, do not try to link against
#   SANDAL2_VERSION_STRING - human-readable string containing the version of SDL_ttf
#
#
#
# For backward compatibility the following variables are also set:
#
# ::
#
#   SANDAL2_LIBRARY (same value as SANDAL2_LIBRARIES)
#   SANDAL2_INCLUDE_DIR (same value as SANDAL2_INCLUDE_DIRS)
#   SANDAL2_FOUND (same value as SANDAL2_FOUND)
#
#
#
# $SDLDIR is an environment variable that would correspond to the
# ./configure --prefix=$SDLDIR used in building SDL.
#
# Created by Eric Wing.  This was influenced by the FindSDL.cmake
# module, but with modifications to recognize OS X frameworks and
# additional Unix paths (FreeBSD, etc).

#=============================================================================
# Copyright 2005-2009 Kitware, Inc.
# Copyright 2012 Benjamin Eikel
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)

find_path(SANDAL2_INCLUDE_DIR SANDAL2.h
        HINTS
        ENV SANDAL2DIR
        PATH_SUFFIXES SANDAL2
        # path suffixes to search inside ENV{SANDAL2DIR}
        include/SANDAL2 include
        PATHS ${SANDAL2_PATH}
        )

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(VC_LIB_PATH_SUFFIX lib/x64)
else ()
    set(VC_LIB_PATH_SUFFIX lib/x86)
endif ()

find_library(SANDAL2_LIBRARY
        NAMES SANDAL2
        HINTS
        ENV SANDAL2DIR
        PATH_SUFFIXES lib ${VC_LIB_PATH_SUFFIX}
        PATHS ${SANDAL2_PATH}
        )

set(SANDAL2_LIBRARIES ${SANDAL2_LIBRARY})
set(SANDAL2_INCLUDE_DIRS ${SANDAL2_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(SANDAL2
        REQUIRED_VARS SANDAL2_LIBRARIES SANDAL2_INCLUDE_DIRS
        VERSION_VAR SANDAL2_VERSION_STRING)

# for backward compatibility
set(SANDAL2_LIBRARY ${SANDAL2_LIBRARIES})
set(SANDAL2_INCLUDE_DIR ${SANDAL2_INCLUDE_DIRS})
