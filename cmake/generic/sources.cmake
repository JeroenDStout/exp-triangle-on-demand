set(cmake_root_dir    ${PROJECT_SOURCE_DIR})
set(cmake_asset_dir   ${sources_root_dir}/assets)
set(cmake_can_src     "${PROJECT_SOURCE_DIR}/src")
set(cmake_can_include "${PROJECT_SOURCE_DIR}/include")
set(cmake_can_script  "${PROJECT_SOURCE_DIR}/script")
set(cmake_gen_src     "${CMAKE_BINARY_DIR}/gen/src")
set(cmake_gen_include "${CMAKE_BINARY_DIR}/gen/include")
set(cmake_gen_script  "${CMAKE_BINARY_DIR}/gen/script")


macro(init_directory name)
  message(STATUS "")
  message(STATUS "-- ${name}")
  
  file(RELATIVE_PATH dir_src_rel ${cmake_root_dir} ${CMAKE_CURRENT_SOURCE_DIR})
  get_filename_component(proj_dir_name ${CMAKE_CURRENT_SOURCE_DIR} NAME)
  string(REGEX REPLACE "src*" "include" dir_include_rel ${dir_src_rel})
  string(REGEX REPLACE "src*" "script"  dir_script_rel  ${dir_src_rel})
  
  set(abs_gen ${CMAKE_BINARY_DIR}/gen)
  
  set(abs_can_src     ${cmake_root_dir}/${dir_src_rel})
  set(abs_gen_src     ${CMAKE_BINARY_DIR}/gen/${dir_src_rel})
  set(abs_can_include ${cmake_root_dir}/${dir_include_rel})
  set(abs_gen_include ${CMAKE_BINARY_DIR}/gen/${dir_include_rel})
  set(abs_can_script  ${cmake_root_dir}/${dir_script_rel})
  set(abs_gen_script  ${CMAKE_BINARY_DIR}/gen/${dir_script_rel})
  file(RELATIVE_PATH rel_can_assets ${abs_can_src} ${cmake_asset_dir})
  
  #message(STATUS "Canononical Source  : ./${abs_can_src}")
  #message(STATUS "Canononical Include : ./${abs_can_include}")
  #message(STATUS "Canononical Script  : ./${abs_can_script}")
  #message(STATUS "Generated Source    : ./${abs_gen_src}")
  #message(STATUS "Generated Include   : ./${abs_gen_include}")
  #message(STATUS "Generated Script    : ./${abs_gen_script}")
endmacro()


macro(init_project name path)
  message(STATUS "Add project ${name}")
  project(${name})
  set(project_folder ${path})
endmacro()


macro(set_project_source_list project_ref)
  # set variable name for project list
  set(project_source_list __sources_${project_ref})
endmacro()


macro(internal_add_project_source mode source)
  # message(STATUS "--- internal_add_project_source (m) ${mode} (1) ${source}")
  
  set(path_name "abs")
  set(mode_list ${mode})
  set(is_generated "no")
  
  if ("generated" IN_LIST mode_list)
    set(path_name "${path_name}_gen")
    set(is_generated "GENERATED")
  else()
    set(path_name "${path_name}_can")
  endif()
  
  if ("include" IN_LIST mode_list)
    set(path_name "${path_name}_include")
  elseif ("script" IN_LIST mode_list)
    set(path_name "${path_name}_script")
  else()
    set(path_name "${path_name}_src")
  endif()
  
  set(src ${${path_name}})
  
  if (NOT ${is_generated} STREQUAL "GENERATED")
    set(new_file ${src}/${source})
    list(APPEND new_files_src ${new_file})
  else()
    set(new_file ${src}/${source})
    set_source_files_properties(${new_file} PROPERTIES GENERATED ON)
    list(APPEND new_files_gen ${new_file})
  endif()
  
  if (NOT EXISTS ${new_file})
    get_filename_component(directory ${new_file} DIRECTORY)
    file(MAKE_DIRECTORY ${directory})
	
    if (NOT ${is_generated} STREQUAL "GENERATED")
      message(STATUS " - Creating ${source}")
      file(TOUCH ${new_file})
    endif()
  endif()
	
  list(APPEND new_files ${new_file})
endmacro()


macro(internal_recursive_setup_project_source_argc)
  if (${ARGC} GREATER 2)
    internal_recursive_setup_project_source(${ARGN})
  elseif (${ARGC} GREATER 1)
	internal_add_project_source(${ARGN})
  endif()
endmacro()


macro(internal_recursive_setup_project_source mode arg1 arg2)
  # message(STATUS "--- internal_recursive_setup_project_source (m) ${mode} (1) ${arg1} (2) ${arg2} (n) ${ARGN}")

  if (${arg1} STREQUAL "src:")
    internal_recursive_setup_project_source_argc("src" ${arg2} ${ARGN})
  elseif (${arg1} STREQUAL "include:")
    internal_recursive_setup_project_source_argc("include" ${arg2} ${ARGN})
  elseif (${arg1} STREQUAL "script:")
    internal_recursive_setup_project_source_argc("script" ${arg2} ${ARGN})
  elseif (NOT ${arg2} STREQUAL "GENERATED")
    internal_add_project_source(${mode} ${arg1})
    internal_recursive_setup_project_source_argc(${project_source_group} ${mode} ${arg2} ${ARGN})
  else()
    internal_add_project_source("${mode};generated" ${arg1})
    internal_recursive_setup_project_source_argc(${project_source_group} ${mode} ${ARGN})
  endif()
endmacro()


macro(setup_project_source project_ref project_source_group)
  set_project_source_list(${project_ref})
  
  set(new_files "")
  set(new_files_src "")
  set(new_files_gen "")
  
  set(mode "")
  
  message(STATUS "Add sources for ${project_ref}/${project_source_group}")
  
  if (${ARGC} GREATER 3)
    internal_recursive_setup_project_source("" ${ARGN})
  else()
    internal_add_project_source("" ${ARGN})
  endif()
  
  # set source group
  IF (new_files_src)
    source_group(${project_source_group} FILES ${new_files_src})
    list(APPEND ${project_source_list} ${new_files_src})
  ENDIF()
  IF (new_files_gen)
    source_group(${project_source_group}/generated FILES ${new_files_gen})
    list(APPEND ${project_source_list} ${new_files_gen})
  ENDIF()
endmacro()


macro(setup_project_source_specific project_ref project_source_group mode)
  set_project_source_list(${project_ref})
  
  set(new_files "")
  
  message(STATUS "Add sources for ${project_ref}/${project_source_group}")
  
  foreach (var IN ITEMS ${ARGN})
    internal_add_project_source("${mode}" ${var})
  endforeach()
  
  # set source group
  source_group(${project_source_group} FILES ${new_files})
  list(APPEND ${project_source_list} ${new_files})
endmacro()


macro(setup_project_source_external_generated project_ref project_source_group)
  set_project_source_list(${project_ref})
  
  set_source_files_properties(${ARGN} PROPERTIES GENERATED ON)
  source_group(${project_source_group}/generated FILES ${ARGN})
  list(APPEND ${project_source_list} ${ARGN})
endmacro()


macro(print_all_project_sources)
  foreach(var IN LISTS ${project_source_list})
    sources_get_canonprint_filepath(${var} rel)
    list(APPEND display_list "${rel}")
  endforeach()
  
  list(SORT display_list)
  
  foreach(var IN LISTS display_list)
    message(STATUS " - ${var}")
  endforeach()
endmacro()


macro(clean_project_source_for_build)
  list(REMOVE_DUPLICATES ${project_source_list})
endmacro()

function(configure_project_executable project_ref)
  set_project_source_list(${project_ref})
  clean_project_source_for_build()
  
  message(STATUS "Configuring executable ${project_ref}")
  print_all_project_sources()
  
  add_executable(${project_ref} ${${project_source_list}})
  target_include_directories(${project_ref} PRIVATE ${cmake_root_dir}/include ${CMAKE_BINARY_DIR}/gen/include)
  
  add_custom_command(
	TARGET     ${project_ref}
    POST_BUILD
    COMMAND    echo * ${project_ref} output $<CONFIG>: $<TARGET_FILE:${project_ref}>
    VERBATIM
  )
  
  set_target_properties(${project_ref} PROPERTIES FOLDER ${project_folder})
  install(TARGETS ${project_ref} DESTINATION ${cmake_root_dir}/bin)
endfunction()


function(configure_project_static_lib project_ref)
  set_project_source_list(${project_ref})
  clean_project_source_for_build()
  
  message(STATUS "Configuring static library ${project_ref}")
  print_all_project_sources()
  
  add_library(${project_ref} STATIC ${${project_source_list}})
  target_include_directories(${project_ref} PRIVATE ${cmake_root_dir}/include ${CMAKE_BINARY_DIR}/gen/include)
  install(TARGETS ${project_ref} DESTINATION ${cmake_root_dir}/bin)
  set_target_properties(${project_ref} PROPERTIES FOLDER ${project_folder})
endfunction()


function(configure_project_script project_ref)
  set_project_source_list(${project_ref})
  clean_project_source_for_build()
  
  message(STATUS "Configuring script project ${project_ref}")
  print_all_project_sources()
  
  add_library(${project_ref} STATIC ${${project_source_list}})
  target_include_directories(${project_ref} PRIVATE ${cmake_root_dir}/include ${CMAKE_BINARY_DIR}/gen/include)
  install(TARGETS ${project_ref} DESTINATION ${cmake_root_dir}/bin)
  set_target_properties(${project_ref} PROPERTIES FOLDER ${project_folder})
  set_target_properties(${project_ref} PROPERTIES LINKER_LANGUAGE CXX)
endfunction()


#
# Utility
#


function(sources_get_canonprint_filepath in_path out_path)  
  if (${in_path} MATCHES "${abs_can_src}*")
    STRING(REGEX REPLACE "^${abs_can_src}/" "src/-/" tmp_path ${in_path})
  elseif (${in_path} MATCHES "${abs_can_include}*")
    STRING(REGEX REPLACE "^${abs_can_include}/" "include/-/" tmp_path ${in_path})
  elseif (${in_path} MATCHES "${abs_can_script}*")
    STRING(REGEX REPLACE "^${abs_can_script}/" "script/-/" tmp_path ${in_path})
  elseif (${in_path} MATCHES "${abs_gen_src}*")
    STRING(REGEX REPLACE "^${abs_gen_src}/" "src/-/" tmp_path "${in_path}*")
  elseif (${in_path} MATCHES "${abs_gen_include}*")
    STRING(REGEX REPLACE "^${abs_gen_include}/" "include/-/" tmp_path "${in_path}*")
  elseif (${in_path} MATCHES "${abs_gen_script}*")
    STRING(REGEX REPLACE "^${abs_gen_script}/" "script/-/" tmp_path "${in_path}*")
  elseif()
    file(RELATIVE_PATH rel ${cmake_root_dir} ${in_path})
    STRING(REGEX REPLACE "^${project_root_dir}/" "      ? " tmp_path "${rel}")
  endif()
  set(${out_path} ${tmp_path} PARENT_SCOPE)
endfunction()


#
# Console
#

function(configure_project_console project_ref)
  message(STATUS "Console is conditional for ${project_ref}")
  
  set_property(TARGET ${project_ref} PROPERTY WIN32_EXECUTABLE $<$<CONFIG:Release>:true>)
endfunction()
