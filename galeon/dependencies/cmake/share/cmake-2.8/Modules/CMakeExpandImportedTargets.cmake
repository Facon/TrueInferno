# CMAKE_EXPAND_IMPORTED_TARGETS(<var> LIBRARIES lib1 lib2...libN
#                                     [CONFIGURATION <config>] )
#
# CMAKE_EXPAND_IMPORTED_TARGETS() takes a list of libraries and replaces
# all imported targets contained in this list with their actual file paths
# of the referenced libraries on disk, including the libraries from their
# link interfaces.
# If a CONFIGURATION is given, it uses the respective configuration of the
# imported targets if it exists. If no CONFIGURATION is given, it uses
# the first configuration from ${CMAKE_CONFIGURATION_TYPES} if set, otherwise
# ${CMAKE_BUILD_TYPE}.
# This macro is used by all Check*.cmake files which use
# try_compile() or try_run() and support CMAKE_REQUIRED_LIBRARIES , so that
# these checks support imported targets in CMAKE_REQUIRED_LIBRARIES:
#    cmake_expand_imported_targets(expandedLibs LIBRARIES ${CMAKE_REQUIRED_LIBRARIES}
#                                               CONFIGURATION "${CMAKE_TRY_COMPILE_CONFIGURATION}" )


#=============================================================================
# Copyright 2012 Kitware, Inc.
# Copyright 2009-2012 Alexander Neundorf <neundorf@kde.org>
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

include(${CMAKE_CURRENT_LIST_DIR}/CMakeParseArguments.cmake)

function(CMAKE_EXPAND_IMPORTED_TARGETS _RESULT )

   set(options )
   set(oneValueArgs CONFIGURATION )
   set(multiValueArgs LIBRARIES )

   cmake_parse_arguments(CEIT "${options}" "${oneValueArgs}" "${multiValueArgs}"  ${ARGN})

   if(CEIT_UNPARSED_ARGUMENTS)
      message(FATAL_ERROR "Unknown keywords given to CMAKE_EXPAND_IMPORTED_TARGETS(): \"${CEIT_UNPARSED_ARGUMENTS}\"")
   endif()

   if(NOT CEIT_CONFIGURATION)
      if(CMAKE_CONFIGURATION_TYPES)
         list(GET CMAKE_CONFIGURATION_TYPES 0 CEIT_CONFIGURATION)
      else()
         set(CEIT_CONFIGURATION ${CMAKE_BUILD_TYPE})
      endif()
   endif()

   # handle imported library targets

   set(_CCSR_REQ_LIBS ${CEIT_LIBRARIES})

   set(_CHECK_FOR_IMPORTED_TARGETS TRUE)
   set(_CCSR_LOOP_COUNTER 0)
   while(_CHECK_FOR_IMPORTED_TARGETS)
      math(EXPR _CCSR_LOOP_COUNTER "${_CCSR_LOOP_COUNTER} + 1 ")
      set(_CCSR_NEW_REQ_LIBS )
      set(_CHECK_FOR_IMPORTED_TARGETS FALSE)
      foreach(_CURRENT_LIB ${_CCSR_REQ_LIBS})
         get_target_property(_importedConfigs "${_CURRENT_LIB}" IMPORTED_CONFIGURATIONS)
         if (_importedConfigs)
#            message(STATUS "Detected imported target ${_CURRENT_LIB}")
            # Ok, so this is an imported target.
            # First we get the imported configurations.
            # Then we get the location of the actual library on disk of the first configuration.
            # then we'll get its link interface libraries property,
            # iterate through it and replace all imported targets we find there
            # with there actual location.

            # guard against infinite loop: abort after 100 iterations ( 100 is arbitrary chosen)
            if ("${_CCSR_LOOP_COUNTER}" LESS 100)
               set(_CHECK_FOR_IMPORTED_TARGETS TRUE)
#                else ()
#                   message(STATUS "********* aborting loop, counter : ${_CCSR_LOOP_COUNTER}")
            endif ()

            # if one of the imported configurations equals ${CMAKE_TRY_COMPILE_CONFIGURATION},
            # use it, otherwise simply use the first one:
            list(FIND _importedConfigs "${CEIT_CONFIGURATION}" _configIndexToUse)
            if("${_configIndexToUse}" EQUAL -1)
              set(_configIndexToUse 0)
            endif()
            list(GET _importedConfigs ${_configIndexToUse} _importedConfigToUse)

            get_target_property(_importedLocation "${_CURRENT_LIB}" IMPORTED_LOCATION_${_importedConfigToUse})
            get_target_property(_linkInterfaceLibs "${_CURRENT_LIB}" IMPORTED_LINK_INTERFACE_LIBRARIES_${_importedConfigToUse} )

            list(APPEND _CCSR_NEW_REQ_LIBS  "${_importedLocation}")
#            message(STATUS "Appending lib ${_CURRENT_LIB} as ${_importedLocation}")
            if(_linkInterfaceLibs)
               foreach(_currentLinkInterfaceLib ${_linkInterfaceLibs})
#                  message(STATUS "Appending link interface lib ${_currentLinkInterfaceLib}")
                  if(_currentLinkInterfaceLib)
                     list(APPEND _CCSR_NEW_REQ_LIBS "${_currentLinkInterfaceLib}" )
                  endif()
               endforeach()
            endif()
         else()
            # "Normal" libraries are just used as they are.
            list(APPEND _CCSR_NEW_REQ_LIBS "${_CURRENT_LIB}" )
#            message(STATUS "Appending lib directly: ${_CURRENT_LIB}")
         endif()
      endforeach()

      set(_CCSR_REQ_LIBS ${_CCSR_NEW_REQ_LIBS} )
   endwhile()

   # Finally we iterate once more over all libraries. This loop only removes
   # all remaining imported target names (there shouldn't be any left anyway).
   set(_CCSR_NEW_REQ_LIBS )
   foreach(_CURRENT_LIB ${_CCSR_REQ_LIBS})
      get_target_property(_importedConfigs "${_CURRENT_LIB}" IMPORTED_CONFIGURATIONS)
      if (NOT _importedConfigs)
         list(APPEND _CCSR_NEW_REQ_LIBS "${_CURRENT_LIB}" )
#         message(STATUS "final: appending ${_CURRENT_LIB}")
      else ()
#             message(STATUS "final: skipping ${_CURRENT_LIB}")
      endif ()
   endforeach()
#   message(STATUS "setting -${_RESULT}- to -${_CCSR_NEW_REQ_LIBS}-")
   set(${_RESULT} "${_CCSR_NEW_REQ_LIBS}" PARENT_SCOPE)

endfunction()
