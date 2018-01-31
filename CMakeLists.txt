# Set minimal cmake version
cmake_minimum_required (VERSION 2.6)

# Set project name
project (gst_test)

# Set putting binaries in bin folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Find hdf5 library
find_package(HDF5)
# Find gstreamer library
find_package(PkgConfig)
pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0)

# Set include dirs
include_directories(${GSTREAMER_INCLUDE_DIRS}
  ${HDF5_INCLUDE_DIRS}
  )

# Parse all c file in src folder
file(GLOB SOURCES src/*.c)
# Loop on each c file for set compilation configuration
FOREACH(SOURCE ${SOURCES})
  # Get filenam base
  string(REGEX MATCH "^(.*)/(.*).c$" dummy ${SOURCE})
  set(SRC ${CMAKE_MATCH_2})
  # Add executable
  add_executable(${SRC} ${SOURCE})
  # Link libraries
  target_link_libraries(${SRC} ${GSTREAMER_LIBRARIES}
    ${HDF5_LIBRARIES})
  # Add compilation flags
  target_compile_options(${SRC} PUBLIC ${GSTREAMER_CFLAGS})
ENDFOREACH(SOURCE)



