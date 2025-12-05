# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\TINYParserGUI_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\TINYParserGUI_autogen.dir\\ParseCache.txt"
  "TINYParserGUI_autogen"
  )
endif()
