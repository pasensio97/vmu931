#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "VMU931::vmu931" for configuration ""
set_property(TARGET VMU931::vmu931 APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(VMU931::vmu931 PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "CXX"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libvmu931.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS VMU931::vmu931 )
list(APPEND _IMPORT_CHECK_FILES_FOR_VMU931::vmu931 "${_IMPORT_PREFIX}/lib/libvmu931.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
