#grpc

ExternalProject_Add(grpc
    PREFIX                  ${PROJECT_BINARY_DIR}/third_party/grpc
    GIT_REPOSITORY          https://github.com/grpc/grpc.git
    GIT_TAG                 v1.37.0
    GIT_SHALLOW             1
    GIT_SUBMODULES_RECURSE  1  
    INSTALL_DIR             install
    CMAKE_ARGS              -DCMAKE_INSTALL_PREFIX:PATH=${PROJECT_BINARY_DIR}/third_party/grpc/install
    #CONFIGURE_COMMAND ""
    #BUILD_COMMAND     ""
    #INSTALL_COMMAND   ""
    #TEST_COMMAND      ""
)


