#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "zxing::zxing" for configuration ""
set_property(TARGET zxing::zxing APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(zxing::zxing PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/bin/zxing"
  )

list(APPEND _IMPORT_CHECK_TARGETS zxing::zxing )
list(APPEND _IMPORT_CHECK_FILES_FOR_zxing::zxing "${_IMPORT_PREFIX}/bin/zxing" )

# Import target "zxing::libzxing" for configuration ""
set_property(TARGET zxing::libzxing APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(zxing::libzxing PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "CXX"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libzxing.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS zxing::libzxing )
list(APPEND _IMPORT_CHECK_FILES_FOR_zxing::libzxing "${_IMPORT_PREFIX}/lib/libzxing.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
