#grpc

ExternalProject_Add(grpc
    PREFIX                  ${PROJECT_BINARY_DIR}/third_party/grpc
    GIT_REPOSITORY          https://github.com/grpc/grpc.git
    GIT_TAG                 v1.37.0
    GIT_SHALLOW             1
    GIT_SUBMODULES_RECURSE  1  
    INSTALL_DIR             install
    CMAKE_ARGS              -DCMAKE_INSTALL_PREFIX:PATH=${PROJECT_BINARY_DIR}/third_party/grpc/install
                            -DBUILD_SHARED_LIBS=FALSE
                            -DgRPC_BUILD_GRPC_CSHARP_PLUGIN=FALSE
                            -DgRPC_BUILD_GRPC_NODE_PLUGIN=FALSE
                            -DgRPC_BUILD_GRPC_OBJECTIVE_C_PLUGIN=FALSE
                            -DgRPC_BUILD_GRPC_PHP_PLUGIN=FALSE
                            -DgRPC_BUILD_GRPC_PYTHON_PLUGIN=FALSE
                            -DgRPC_BUILD_GRPC_RUBY_PLUGIN=FALSE
                            -DgRPC_BUILD_TESTS=FALSE
                            -Dprotobuf_BUILD_EXAMPLES=FALSE
                            -Dprotobuf_BUILD_TESTS=FALSE
                            -DCARES_BUILD_TOOLS=FALSE
                            -DCARES_BUILD_TESTS=FALSE
                            -DCARES_STATIC=TRUE
                            -DgRPC_INSTALL=TRUE
                            -DgRPC_MSVC_STATIC_RUNTIME=TRUE
                            -Dprotobuf_MSVC_STATIC_RUNTIME=TRUE
    #CONFIGURE_COMMAND ""
    #BUILD_COMMAND     ""
    #INSTALL_COMMAND   ""
    #TEST_COMMAND      ""
)


