#=============================================================================
#  OOMidi
#  OpenOctave Midi and Audio Editor
#  $Id:$
#
#  Copyright (C) 2002-2006 by Werner Schweer and others
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License version 2.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#=============================================================================

##
## Expand Qt macros in source files
##
QT4_WRAP_CPP ( mixer_mocs
      AudioMixer.h
      astrip.h
      auxknob.h
      meter.h
	  #mstrip.h
      panknob.h
	  #rack.h
      strip.h
      )

##
## UI files
##
file (GLOB mixer_ui_files
      )
QT4_WRAP_UI ( mixer_uis ${mixer_ui_files} )

##
## List of source files to compile
##
file (GLOB mixer_source_files
      AudioMixer.cpp  
      astrip.cpp
      auxknob.cpp 
      meter.cpp 
	  #mstrip.cpp
      panknob.cpp 
	  #rack.cpp 
      strip.cpp
      )

##
## Define target
##
add_library ( mixer STATIC
      ${mixer_source_files}
      ${mixer_mocs}
      ${mixer_uis}
      )

##
## Append to the list of translations
##
set (FILES_TO_TRANSLATE
      ${FILES_TO_TRANSLATE}
      ${mixer_source_files}
      ${mixer_ui_files}
      CACHE INTERNAL ""
      )

##
## Compilation flags and target name
##
set_target_properties ( mixer
      PROPERTIES COMPILE_FLAGS "-include ${PROJECT_BINARY_DIR}/all.h -fPIC"
      OUTPUT_NAME oom_mixer
      )

##
## Linkage
##
target_link_libraries ( mixer
      ${QT_LIBRARIES}
      widgets
      )

##
## Install location
##
#install(TARGETS mixer
#     DESTINATION ${OOMidi_MODULES_DIR}
#     )


