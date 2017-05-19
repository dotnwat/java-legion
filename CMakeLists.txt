cmake_minimum_required(VERSION 3.1)
project(java-legion)

find_package(Java COMPONENTS Development)
find_package(JNI REQUIRED)
include(UseJava)

add_subdirectory(legion)

set(JAVA_CLASSES
  org.legion.Runtime
  org.legion.LegionObject
  org.legion.Future
  org.legion.Context
  org.legion.Task
  org.legion.IndexSpace
  org.legion.FieldSpace
  org.legion.FieldAllocator
  org.legion.LogicalRegion
  org.legion.RegionRequirement
  org.legion.InlineLauncher
  org.legion.PhysicalRegion
  org.legion.IndexAllocator
  org.legion.Pointer
  org.legion.Point
  org.legion.Rect
  org.legion.Domain
  org.legion.DomainColoring
  org.legion.IndexPartition
  org.legion.LogicalPartition
  org.legion.ArgumentMap
  org.legion.TaskLauncher
  org.legion.IndexLauncher
  org.legion.FutureMap)

file(GLOB_RECURSE JAVA_SOURCES "src/main/java/org/legion/*.java")

add_jar(legion_jar ${JAVA_SOURCES} OUTPUT_NAME legion)

add_custom_target(make_jni_includes_dir
    ALL COMMAND ${CMAKE_COMMAND} -E make_directory
    ${CMAKE_CURRENT_BINARY_DIR}/include)
  
create_javah(TARGET jni_headers
  DEPENDS make_jni_includes_dir
  CLASSES ${JAVA_CLASSES}
  CLASSPATH legion_jar
  OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/include)

add_library(legion_jni SHARED
  native/runtime.cc
  native/region_requirement.cc
  native/logical_region.cc
  native/inline_launcher.cc
  native/physical_region.cc
  native/task_launcher.cc
  native/future.cc
  native/field_allocator.cc
  native/index_space.cc
  native/field_space.cc
  native/point.cc
  native/rect.cc
  native/domain.cc
  native/domain_coloring.cc
  native/index_partition.cc
  native/logical_partition.cc
  native/argument_map.cc
  native/index_allocator.cc
  native/index_launcher.cc
  native/future_map.cc
  native/task.cc)

add_dependencies(legion_jni jni_headers)

if(APPLE)
  set_target_properties(legion_jni PROPERTIES OUTPUT_NAME legionjni-osx)
  set_target_properties(legion_jni PROPERTIES SUFFIX .jnilib)
endif(APPLE)

set_target_properties(legion_jni PROPERTIES OUTPUT_NAME legionjni-linux64)

target_include_directories(legion_jni
  PUBLIC ${CMAKE_CURRENT_BINARY_DIR}
  PUBLIC ${JNI_INCLUDE_DIRS})

target_link_libraries(legion_jni
  ${JNI_LIBRARIES} Legion)

get_property(LegionJarFile TARGET legion_jar PROPERTY JAR_FILE)
add_custom_command(TARGET legion_jni POST_BUILD
  COMMAND "${Java_JAR_EXECUTABLE}" -uf ${LegionJarFile} -C $<TARGET_FILE_DIR:legion_jni> $<TARGET_FILE_NAME:legion_jni>)
