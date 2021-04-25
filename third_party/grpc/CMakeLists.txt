#grpc

#https://forums.accellera.org/topic/5777-cmake-install-both-debug-and-release-builds-of-systemc/

#[[
ExternalProject_Add(grpc
    PREFIX                  ${PROJECT_BINARY_DIR}/third_party/grpc
    GIT_REPOSITORY          https://github.com/grpc/grpc.git
    GIT_TAG                 v1.37.0
    GIT_SHALLOW             1
    GIT_SUBMODULES_RECURSE  1
    BUILD_IN_SOURCE         0
    BINARY_DIR              build
    INSTALL_DIR             install
    SOURCE_DIR              repo

    #CMAKE_ARGS
    #CMAKE_CACHE_ARGS
    
    CONFIGURE_COMMAND       ${CMAKE_COMMAND} -S${PROJECT_BINARY_DIR}/third_party/grpc/repo -B${PROJECT_BINARY_DIR}/third_party/grpc/build/$<CONFIG>
                            -DCMAKE_INSTALL_PREFIX:PATH=${PROJECT_BINARY_DIR}/third_party/grpc/install/$<CONFIG> -DCMAKE_BUILD_TYPE=$<CONFIG>
                            -DCMAKE_INSTALL_CONFIG_NAME=$<CONFIG>
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
                            -DABSL_RUN_TESTS:BOOL=OFF
                            -DABSL_ENABLE_INSTALL:BOOL=ON  #see bug: https://github.com/grpc/grpc/issues/24976
                            -DCARES_BUILD_TOOLS:BOOL=OFF
                            -DCARES_BUILD_TESTS:BOOL=OFF
                            -DCARES_STATIC:BOOL=ON
                            -DCARES_MSVC_STATIC_RUNTIME:BOOL=ON
                            -Dprotobuf_BUILD_EXAMPLES:BOOL=OFF
                            -Dprotobuf_BUILD_TESTS:BOOL=OFF
                            -Dprotobuf_MSVC_STATIC_RUNTIME:BOOL=ON

    BUILD_COMMAND           ${CMAKE_COMMAND} --build ${PROJECT_BINARY_DIR}/third_party/grpc/build/$<CONFIG> --config $<CONFIG>
    INSTALL_COMMAND         ${CMAKE_COMMAND} --install ${PROJECT_BINARY_DIR}/third_party/grpc/build/$<CONFIG>
    #TEST_COMMAND           ""
)
]]


# Builds absl project from the git submodule.
ExternalProject_Add(absl
  PREFIX absl
  SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../../third_party/abseil-cpp"
  CMAKE_CACHE_ARGS
        -DCMAKE_POSITION_INDEPENDENT_CODE:BOOL=TRUE
        -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/absl
)

# Builds c-ares project from the git submodule.
ExternalProject_Add(c-ares
  PREFIX c-ares
  SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../../third_party/cares/cares"
  CMAKE_CACHE_ARGS
        -DCARES_SHARED:BOOL=OFF
        -DCARES_STATIC:BOOL=ON
        -DCARES_STATIC_PIC:BOOL=ON
        -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/c-ares
)

# Builds protobuf project from the git submodule.
ExternalProject_Add(protobuf
  PREFIX protobuf
  SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../../third_party/protobuf/cmake"
  CMAKE_CACHE_ARGS
        -Dprotobuf_BUILD_TESTS:BOOL=OFF
        -Dprotobuf_WITH_ZLIB:BOOL=OFF
        -Dprotobuf_MSVC_STATIC_RUNTIME:BOOL=OFF
        -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/protobuf
)

# Builds re2 project from the git submodule.
ExternalProject_Add(re2
  PREFIX re2
  SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../../third_party/re2"
  CMAKE_CACHE_ARGS
        -DCMAKE_POSITION_INDEPENDENT_CODE:BOOL=TRUE
        -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/re2
)

# Builds zlib project from the git submodule.
ExternalProject_Add(zlib
  PREFIX zlib
  SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../../third_party/zlib"
  CMAKE_CACHE_ARGS
        -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/zlib
)

# the location where protobuf-config.cmake will be installed varies by platform
if (WIN32)
  set(_FINDPACKAGE_PROTOBUF_CONFIG_DIR "${CMAKE_CURRENT_BINARY_DIR}/protobuf/cmake")
else()
  set(_FINDPACKAGE_PROTOBUF_CONFIG_DIR "${CMAKE_CURRENT_BINARY_DIR}/protobuf/lib/cmake/protobuf")
endif()

# if OPENSSL_ROOT_DIR is set, propagate that hint path to the external projects with OpenSSL dependency.
set(_CMAKE_ARGS_OPENSSL_ROOT_DIR "")
if (OPENSSL_ROOT_DIR)
  set(_CMAKE_ARGS_OPENSSL_ROOT_DIR "-DOPENSSL_ROOT_DIR:PATH=${OPENSSL_ROOT_DIR}")
endif()

# Builds gRPC based on locally checked-out sources and set arguments so that all the dependencies
# are correctly located.
ExternalProject_Add(grpc
  PREFIX grpc
  SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../.."
  CMAKE_CACHE_ARGS
        -DgRPC_INSTALL:BOOL=ON
        -DgRPC_BUILD_TESTS:BOOL=OFF
        -DgRPC_PROTOBUF_PROVIDER:STRING=package
        -DgRPC_PROTOBUF_PACKAGE_TYPE:STRING=CONFIG
        -DProtobuf_DIR:PATH=${_FINDPACKAGE_PROTOBUF_CONFIG_DIR}
        -DgRPC_RE2_PROVIDER:STRING=package
        -Dre2_DIR:STRING=${CMAKE_CURRENT_BINARY_DIR}/re2/lib/cmake/re2
        -DgRPC_ZLIB_PROVIDER:STRING=package
        -DZLIB_ROOT:STRING=${CMAKE_CURRENT_BINARY_DIR}/zlib
        -DgRPC_ABSL_PROVIDER:STRING=package
        -Dabsl_DIR:STRING=${CMAKE_CURRENT_BINARY_DIR}/absl/lib/cmake/absl
        -DgRPC_CARES_PROVIDER:STRING=package
        -Dc-ares_DIR:PATH=${CMAKE_CURRENT_BINARY_DIR}/c-ares/lib/cmake/c-ares
        -DgRPC_SSL_PROVIDER:STRING=package
        ${_CMAKE_ARGS_OPENSSL_ROOT_DIR}
        -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/grpc
  DEPENDS c-ares protobuf re2 zlib absl
)

