#
# Initial set-up
#

cmake_minimum_required(VERSION 3.15)

# Packages
# include(cmake/generic/vcpkg.cmake)

# Project
project(triangle_on_demand)
include(cmake/generic/publish.cmake)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Languages
include(cmake/generic/cxx.cmake)
include(cmake/generic/python.cmake)
# Third party
include(cmake/generic/sdl_3.cmake)

# Support
include(cmake/generic/artefacts.cmake)
include(cmake/generic/sources.cmake)

# Subprojects
add_subdirectory(src/repo_version)

# Publication
finalise_publish()
