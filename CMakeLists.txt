# 
# CMake options
# 

# CMake version
cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

#
# Configure CMake environment
#

# Register general cmake commands
include(cmake/Custom.cmake)

# Set policies
set_policy(CMP0054 NEW) # ENABLE CMP0054: Only interpret if() arguments as variables or keywords when unquoted.
set_policy(CMP0042 NEW) # ENABLE CMP0042: MACOSX_RPATH is enabled by default.
set_policy(CMP0063 NEW) # ENABLE CMP0063: Honor visibility properties for all target types.
set_policy(CMP0077 NEW) # ENABLE CMP0077: option() honors normal variables
set_policy(CMP0091 NEW) # ENABLE CMP0091: 


# Remove ZERO_CHECK target
#set(CMAKE_SUPPRESS_REGENERATION true)

# Include cmake modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(GenerateExportHeader)

set(WriterCompilerDetectionHeaderFound NOTFOUND)
# This module is only available with CMake >=3.1, so check whether it could be found
# BUT in CMake 3.1 this module doesn't recognize AppleClang as compiler, so just use it as of CMake 3.2
if (${CMAKE_VERSION} VERSION_GREATER "3.2")
    include(WriteCompilerDetectionHeader OPTIONAL RESULT_VARIABLE WriterCompilerDetectionHeaderFound)
endif()

# Include custom cmake modules
include(cmake/GenerateTemplateExportHeader.cmake)
include(cmake/GetGitRevisionDescription.cmake)


# 
# Project description and (meta) information
# 

# Get git revision
get_git_head_revision(GIT_REFSPEC GIT_SHA1)
string(SUBSTRING "${GIT_SHA1}" 0 12 GIT_REV)
if(NOT GIT_SHA1)
    set(GIT_REV "0")
endif()

# Meta information about the project
set(META_PROJECT_NAME        "grpc_service")
set(META_PROJECT_DESCRIPTION "gRPC service template project")
set(META_AUTHOR_ORGANIZATION "My Company")
set(META_AUTHOR_DOMAIN       "https://github.com/vstanchevici/grpc_service")
set(META_AUTHOR_MAINTAINER   "vstanchevici@email.com")
set(META_VERSION_MAJOR       "1")
set(META_VERSION_MINOR       "0")
set(META_VERSION_PATCH       "0")
set(META_VERSION_REVISION    "${GIT_REV}")
set(META_VERSION             "${META_VERSION_MAJOR}.${META_VERSION_MINOR}.${META_VERSION_PATCH}")
set(META_NAME_VERSION        "${META_PROJECT_NAME} v${META_VERSION} (${META_VERSION_REVISION})")
set(META_CMAKE_INIT_SHA      "${GIT_REV}")

string(MAKE_C_IDENTIFIER ${META_PROJECT_NAME} META_PROJECT_ID)
string(TOUPPER ${META_PROJECT_ID} META_PROJECT_ID)

# 
# Project configuration options
# 

# Project options
#option(BUILD_SHARED_LIBS      "Build shared instead of static libraries."              ON)
#option(OPTION_SELF_CONTAINED  "Create a self-contained install with all dependencies." OFF)
#option(OPTION_BUILD_TESTS     "Build tests."                                           ON)
#option(OPTION_BUILD_DOCS      "Build documentation."                                   OFF)
#option(OPTION_BUILD_EXAMPLES  "Build examples."                                        OFF)
#option(OPTION_ENABLE_COVERAGE "Add coverage information."                              OFF)

# 
# Declare project
# 

# Generate folders for IDE targets (e.g., VisualStudio solutions)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(IDE_FOLDER "")

# Declare project
project(${META_PROJECT_NAME} LANGUAGES C CXX)

# Set standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

# Create version file
file(WRITE "${PROJECT_BINARY_DIR}/VERSION" "${META_NAME_VERSION}")

# Set src and include root path
set(SRC_ROOT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/source/")
set(INCLUDE_ROOT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/include/")

# Generate version-header
configure_file(${INCLUDE_ROOT_PATH}/version.h.in ${INCLUDE_ROOT_PATH}/version.h @ONLY)

file(
    GLOB_RECURSE INCLUDE_LIST
    LIST_DIRECTORIES false
    "${INCLUDE_ROOT_PATH}/*.hpp"
    "${INCLUDE_ROOT_PATH}/*.h"
)

file(
    GLOB_RECURSE SOURCE_LIST
    LIST_DIRECTORIES false
    "${SRC_ROOT_PATH}/*.c"
    "${SRC_ROOT_PATH}/*.cpp"
    "${SRC_ROOT_PATH}/*.h"
    "${SRC_ROOT_PATH}/*.hpp"
)

foreach(_include IN ITEMS ${INCLUDE_LIST})
    get_filename_component(_include_path "${_include}" PATH)
    file(RELATIVE_PATH _include_path_rel "${INCLUDE_ROOT_PATH}" "${_include_path}")
    string(REPLACE "/" "\\" _group_path2 "${_include_path_rel}")
    source_group("include\\${_group_path2}" FILES "${_include}")
endforeach()

foreach(_source IN ITEMS ${SOURCE_LIST})
    get_filename_component(_source_path "${_source}" PATH)
    file(RELATIVE_PATH _source_path_rel "${SRC_ROOT_PATH}" "${_source_path}")
    string(REPLACE "/" "\\" _group_path "${_source_path_rel}")
    source_group("source\\${_group_path}" FILES "${_source}")
endforeach()


add_executable(${PROJECT_NAME} ${INCLUDE_LIST} ${SOURCE_LIST})
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_ROOT_PATH} ${SRC_ROOT_PATH})


# test mscv
#https://stackoverflow.com/questions/10046114/in-cmake-how-can-i-test-if-the-compiler-is-clang

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(GRPC_SERVICE_MSVC_RUNTIME_LIBRARY "MultiThreadedStatic" CACHE STRING "Select the MSVC runtime library for use by compiler.")
    
    set_property(CACHE GRPC_SERVICE_MSVC_RUNTIME_LIBRARY PROPERTY STRINGS "MultiThreadedShared;MultiThreadedStatic")

    if(${GRPC_SERVICE_MSVC_RUNTIME_LIBRARY} STREQUAL "MultiThreadedShared")
        message(STATUS "runtime shared")
        set_property(TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
    endif()

    if(${GRPC_SERVICE_MSVC_RUNTIME_LIBRARY} STREQUAL "MultiThreadedStatic")
        message(STATUS "runtime static")
        set_property(TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    endif()
endif()


#
# Start adding libraries
#

#include path
if (DEFINED LIBRARIES_INSTALL_PREFIX)
    list(APPEND CMAKE_PREFIX_PATH ${LIBRARIES_INSTALL_PREFIX}/grpc)
    message(STATUS "CMAKE_PREFIX_PATH: " ${CMAKE_PREFIX_PATH})
endif()
    
# Find Protobuf installation
# Looks for protobuf-config.cmake file installed by Protobuf's cmake installation.
set(protobuf_MODULE_COMPATIBLE TRUE)
find_package(Protobuf CONFIG)
message(STATUS "Using protobuf ${Protobuf_VERSION}")

set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
set(_REFLECTION gRPC::grpc++_reflection)
if(CMAKE_CROSSCOMPILING)
    find_program(_PROTOBUF_PROTOC protoc)
else()
    set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)
endif()

# Find gRPC installation
# Looks for gRPCConfig.cmake file installed by gRPC's cmake installation.
find_package(gRPC CONFIG REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")

set(_GRPC_GRPCPP gRPC::grpc++)
if(CMAKE_CROSSCOMPILING)
    find_program(_GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)
else()
    set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)
endif()    



find_package(Threads REQUIRED)

# GRPC service Proto files
set(GRPC_SERVICE_PROTO_ROOT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/proto/grpc")
file(RELATIVE_PATH GRPC_SERVICE_PROTO_RELATIVE_PATH "${CMAKE_CURRENT_SOURCE_DIR}" "${GRPC_SERVICE_PROTO_ROOT_PATH}")
set(GRPC_SERVICE_PROTO_GENERATED_PATH "${CMAKE_CURRENT_BINARY_DIR}/generated/${GRPC_SERVICE_PROTO_RELATIVE_PATH}")

file(
    GLOB_RECURSE PROTO_LIST
    LIST_DIRECTORIES false
    "${GRPC_SERVICE_PROTO_ROOT_PATH}/*.proto"
)

add_custom_target(proto SOURCES ${PROTO_LIST})
set_source_files_properties(${PROTO_LIST} PROPERTIES HEADER_FILE_ONLY TRUE)


foreach(_proto IN ITEMS ${PROTO_LIST})
    get_filename_component(_proto_name "${_proto}" NAME_WLE)
    get_filename_component(_proto_path "${_proto}" PATH)
    file(RELATIVE_PATH _rel_path "${GRPC_SERVICE_PROTO_ROOT_PATH}" "${_proto_path}")
    #string(REPLACE "/" "\\" _rel_path "${_rel_path}")
    source_group("grpc_services\\${_rel_path}" FILES "${_proto}")

    file(MAKE_DIRECTORY ${GRPC_SERVICE_PROTO_GENERATED_PATH}/${_rel_path})
    
    #message(STATUS "_proto->" ${_proto})
    #message(STATUS "_proto_path->" ${_proto_path})
    #message(STATUS "_rel_path->" ${_rel_path})
    #message(STATUS "_gen_path->" ${GRPC_SERVICE_PROTO_GENERATED_PATH}/${_rel_path})
    

    # Generated sources
    set(_proto_srcs ${GRPC_SERVICE_PROTO_GENERATED_PATH}/${_rel_path}/${_proto_name}.pb.cc)
    set(_proto_hdrs ${GRPC_SERVICE_PROTO_GENERATED_PATH}/${_rel_path}/${_proto_name}.pb.h)
    set(_grpc_srcs ${GRPC_SERVICE_PROTO_GENERATED_PATH}/${_rel_path}/${_proto_name}.grpc.pb.cc)
    set(_grpc_hdrs ${GRPC_SERVICE_PROTO_GENERATED_PATH}/${_rel_path}/${_proto_name}.grpc.pb.h)
    
    #message(STATUS "_proto_srcs->" ${_proto_srcs})

    source_group("generated/grpc_services/${_rel_path}" FILES "${_proto_srcs}")
    source_group("generated/grpc_services/${_rel_path}" FILES "${_grpc_srcs}")
    source_group("generated/grpc_services/${_rel_path}" FILES "${_proto_hdrs}")
    source_group("generated/grpc_services/${_rel_path}" FILES "${_grpc_hdrs}")

    add_custom_command(
          COMMENT "Running ${_PROTOBUF_PROTOC} for ${_proto_name}.proto"
          OUTPUT "${_proto_srcs}" "${_proto_hdrs}" "${_grpc_srcs}" "${_grpc_hdrs}"
          COMMAND ${_PROTOBUF_PROTOC}
          ARGS --grpc_out "${GRPC_SERVICE_PROTO_GENERATED_PATH}/${_rel_path}"
            --cpp_out "${GRPC_SERVICE_PROTO_GENERATED_PATH}/${_rel_path}"
            -I "${_proto_path}"
            --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
            "${_proto}"
          DEPENDS "${_proto}")
 
    target_sources(${PROJECT_NAME} PRIVATE ${_proto_srcs} ${_proto_hdrs} ${_grpc_srcs} ${_grpc_hdrs})    
endforeach()

# Include generated *.pb.h files
#message(STATUS "GRPC_SERVICE_PROTO_GENERATED_PATH: " ${GRPC_SERVICE_PROTO_GENERATED_PATH})
target_include_directories(${PROJECT_NAME} PRIVATE ${GRPC_SERVICE_PROTO_GENERATED_PATH})

target_link_libraries(${PROJECT_NAME} ${_REFLECTION} ${_GRPC_GRPCPP} ${_PROTOBUF_LIBPROTOBUF})


# Install version file
#message(STATUS "some message ...")
#set(INSTALL_ROOT      "./test/")       # /usr/[local]/share/<project>
#install(FILES "${PROJECT_BINARY_DIR}/VERSION" DESTINATION ${INSTALL_ROOT} COMPONENT runtime)
install(TARGETS ${PROJECT_NAME} DESTINATION ./)
