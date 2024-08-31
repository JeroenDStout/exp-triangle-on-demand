message(STATUS "Setup vcpkg")

set(CMAKE_TOOLCHAIN_FILE "c:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

set(vcpkg_lib_dir   "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/")
set(vcpkg_lib_dir_d "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/debug/lib")
