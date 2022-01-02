# CMake version
cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project(grpc_service-superbuild)

# Set standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

include(ProcessorCount)
ProcessorCount(CPU_COUNT)
if(CPU_COUNT EQUAL 0)
    set(CPU_COUNT 1)
endif()
math(EXPR CPU_COUNT "${CPU_COUNT} / 2 + 1")


include(ExternalProject)

if(NOT DEFINED LIBRARIES_PREFIX)
    set(LIBRARIES_PREFIX ${CMAKE_BINARY_DIR})
endif()

if(NOT DEFINED LIBRARIES_INSTALL_PREFIX)
    set(LIBRARIES_INSTALL_PREFIX ${LIBRARIES_PREFIX}/installed/${CMAKE_SYSTEM_NAME})
endif()

if (WIN32)
    set(SHELL_EXTENSION ".bat")
    set(EXE_EXTENSION ".exe")
elseif (UNIX)
    set(SHELL_EXTENSION ".sh")
    set(EXE_EXTENSION "")
endif()


ExternalProject_Add(grpc
    PREFIX                  ${LIBRARIES_PREFIX}/grpc
    GIT_REPOSITORY          https://github.com/grpc/grpc.git
    GIT_TAG                 v1.41.0
    GIT_SHALLOW             ON
    GIT_SUBMODULES_RECURSE  ON 
    BUILD_ALWAYS            FALSE
    UPDATE_DISCONNECTED     TRUE
    UPDATE_COMMAND          ""

    #CMAKE_ARGS              -DCARES_MSVC_STATIC_RUNTIME=ON -DCARES_INSTALL=ON -DCMAKE_INSTALL_PREFIX:PATH=${LIBRARIES_INSTALL_PREFIX}/$<CONFIG>/grpc
    CMAKE_ARGS              -DCMAKE_INSTALL_PREFIX:PATH=${LIBRARIES_INSTALL_PREFIX}/grpc
                            -DCMAKE_DEBUG_POSTFIX=$<$<CONFIG:Debug>:d>
                            -DBUILD_SHARED_LIBS:BOOL=OFF
                            -DgRPC_BUILD_GRPC_CSHARP_PLUGIN:BOOL=OFF
                            -DgRPC_BUILD_GRPC_NODE_PLUGIN:BOOL=OFF
                            -DgRPC_BUILD_GRPC_OBJECTIVE_C_PLUGIN:BOOL=OFF
                            -DgRPC_BUILD_GRPC_PHP_PLUGIN:BOOL=OFF
                            -DgRPC_BUILD_GRPC_PYTHON_PLUGIN:BOOL=OFF
                            -DgRPC_BUILD_GRPC_RUBY_PLUGIN:BOOL=OFF
                            -DgRPC_BUILD_GRPC_CPP_PLUGIN:BOOL=ON
                            -DgRPC_BUILD_CODEGEN:BOOL=ON
                            -DgRPC_BUILD_TESTS:BOOL=OFF
                            -DgRPC_MSVC_STATIC_RUNTIME:BOOL=ON
                            -DgRPC_INSTALL:BOOL=ON
                            -DCARES_BUILD_TOOLS:BOOL=OFF
                            -DCARES_BUILD_TESTS:BOOL=OFF
                            -DCARES_STATIC:BOOL=ON
                            -DCARES_MSVC_STATIC_RUNTIME:BOOL=ON
                            -Dprotobuf_BUILD_EXAMPLES:BOOL=OFF
                            -Dprotobuf_BUILD_TESTS:BOOL=OFF
                            -Dprotobuf_MSVC_STATIC_RUNTIME:BOOL=ON
                            -DOPENSSL_NO_ASM:BOOL=ON #bug on windows

    #CONFIGURE_COMMAND ""
    #BUILD_COMMAND     ""
    #INSTALL_COMMAND   ""
    #TEST_COMMAND      ""
)

ExternalProject_Add (grpc_service
    PREFIX              ${LIBRARIES_PREFIX}/grpc_service
    DEPENDS             grpc
    SOURCE_DIR          ${PROJECT_SOURCE_DIR}/../
    CMAKE_ARGS          -DLIBRARIES_INSTALL_PREFIX=${LIBRARIES_INSTALL_PREFIX} -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
    BUILD_ALWAYS        TRUE
    INSTALL_COMMAND     ""
)

