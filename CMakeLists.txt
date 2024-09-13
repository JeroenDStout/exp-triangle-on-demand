#
# Initial set-up
#

cmake_minimum_required(VERSION 3.15)

# Packages
# include(cmake/generic/vcpkg.cmake)

# Project
project(triangle_on_demand)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Languages
include(cmake/generic/cxx.cmake)
include(cmake/generic/python.cmake)

# Third party
include(cmake/generic/eigen.cmake)
include(cmake/generic/sdl_3.cmake)
include(cmake/generic/nanobind.cmake)

# Support
include(cmake/generic/outputs.cmake)
include(cmake/generic/artefacts.cmake)
include(cmake/generic/sources.cmake)
include(cmake/generic/assets.cmake)
include(cmake/generic/publish.cmake)

# Subprojects
add_subdirectory(src/tod_core)
add_subdirectory(src/tod_live)
add_subdirectory(src/tod_py)
add_subdirectory(src/repo_version)

# Publication
finalise_publish()
