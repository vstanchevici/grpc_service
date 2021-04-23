#grpc

ExternalProject_Add(grpc
    PREFIX                  ${PROJECT_BINARY_DIR}/third_party/grpc
    GIT_REPOSITORY          https://github.com/grpc/grpc.git
    GIT_TAG                 v1.37.0
    GIT_SHALLOW             1
    GIT_SUBMODULES_RECURSE  1  
    INSTALL_DIR             install
    CMAKE_ARGS              -DCMAKE_INSTALL_PREFIX:PATH=${PROJECT_BINARY_DIR}/third_party/grpc/install
            
    CMAKE_CACHE_ARGS        -DBUILD_SHARED_LIBS:BOOL=OFF
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
                            -DABSL_ENABLE_INSTALL:BOOL=OFF  #see bug: https://github.com/grpc/grpc/issues/24976
                            -DCARES_BUILD_TOOLS:BOOL=OFF
                            -DCARES_BUILD_TESTS:BOOL=OFF
                            -DCARES_STATIC:BOOL=ON
                            -DCARES_MSVC_STATIC_RUNTIME:BOOL=ON
                            -Dprotobuf_BUILD_EXAMPLES:BOOL=OFF
                            -Dprotobuf_BUILD_TESTS:BOOL=OFF
                            -Dprotobuf_MSVC_STATIC_RUNTIME:BOOL=ON
    #CONFIGURE_COMMAND ""
    #BUILD_COMMAND     ""
    #INSTALL_COMMAND   ""
    #TEST_COMMAND      ""
)


