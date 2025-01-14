# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/_cluster_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/_cluster_autogen.dir/ParseCache.txt"
  "_cluster_autogen"
  )
endif()
