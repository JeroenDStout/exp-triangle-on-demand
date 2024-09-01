message(STATUS "Setup Eigen")

include(FetchContent)
cmake_policy(SET CMP0135 NEW)

FetchContent_Declare(
  eigen
  GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
  GIT_TAG master
  GIT_SHALLOW TRUE
  GIT_PROGRESS TRUE)
# note: To disable eigen tests,
# you should put this code in a add_subdirectory to avoid to change
# BUILD_TESTING for your own project too since variables are directory
# scoped
set(BUILD_TESTING OFF)
set(EIGEN_BUILD_TESTING OFF)
set(EIGEN_MPL2_ONLY ON)
set(EIGEN_BUILD_PKGCONFIG OFF)
set(EIGEN_BUILD_DOC OFF)
FetchContent_MakeAvailable(eigen)

function(configure_project_eigen project_ref)
  include_directories(${eigen_SOURCE_DIR})
  target_link_libraries(${project_ref} PRIVATE eigen)
  target_compile_options(${project_ref}
    PRIVATE
      "/wd5031;" # pragma warning push pop mismatch
      "/wd4464;" # relative include path contains '..'
      "/wd5266;" # 'const' qualifier on return type has no effect
)
endfunction()