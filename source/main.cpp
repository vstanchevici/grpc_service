#include <version.h>
#include <iostream>
#include <filesystem>
#include <vector>
#include <s3/httpexample.grpc.pb.h>

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
//#include <grpcpp/health_check_service_interface.h>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using httpexample::HTTPExample;
using httpexample::HTTPEReply;
using httpexample::HTTPERequest;

class HTTPExampleServiceImpl final : public HTTPExample::Service
{
    
    Status SayHello(ServerContext* context, const HTTPERequest* request, HTTPEReply* reply) override
    {
        std::string prefix("Hello ");
        std::cout << request->name() << std::endl;
        reply->set_message(prefix + request->name());
        return Status::OK;
    }
};

void RunServer() {
    std::string server_address("0.0.0.0:8080");
    HTTPExampleServiceImpl service;

    //grpc::EnableDefaultHealthCheckService(true);
    //grpc::reflection::InitProtoReflectionServerBuilderPlugin();
    ServerBuilder builder;
    // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    // Register "service" as the instance through which we'll communicate with
    // clients. In this case it corresponds to an *synchronous* service.
    builder.RegisterService(&service);
    // Finally assemble the server.
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    // Wait for the server to shutdown. Note that some other thread must be
    // responsible for shutting down the server for this call to ever return.
    server->Wait();
}

int main(int argc, char** argv)
{
    RunServer();

    return 0;
}
