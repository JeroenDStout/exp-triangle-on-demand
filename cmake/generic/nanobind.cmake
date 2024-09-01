message(STATUS "Setup Nanobind")

include(FetchContent)
cmake_policy(SET CMP0135 NEW)

FetchContent_Declare(
  nanobind
  GIT_REPOSITORY https://github.com/wjakob/nanobind.git
  GIT_TAG        8d7f1ee0621c17fa370b704b2100ffa0243d5bfb
)
FetchContent_MakeAvailable(nanobind)
find_package(nanobind CONFIG REQUIRED)
  
function(configure_project_nanobind project_ref)
  include_directories(${nanobind_SOURCE_DIR}/include)
  
  set_project_source_list(${project_ref})
  clean_project_source_for_build()
  
  message(STATUS "Configuring nanobind project ${project_ref}")
  print_all_project_sources()
  
  nanobind_add_module(${project_ref} ${${project_source_list}})
  target_include_directories(${project_ref} PRIVATE ${cmake_can_include} ${CMAKE_BINARY_DIR}/gen/include)
  install(TARGETS ${project_ref} DESTINATION ${project_root_dir}/bin)
  set_target_properties(${project_ref} PROPERTIES FOLDER ${project_folder})
  set_target_properties(${project_ref} PROPERTIES LINKER_LANGUAGE CXX)
  
  set_target_properties(nanobind-static PROPERTIES FOLDER "dependencies/nanobind")
  target_link_libraries(nanobind-static PRIVATE Eigen3::Eigen)
endfunction()
