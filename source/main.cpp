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

/*
const char descriptor_table_protodef_annotations_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
"\n\021annotations.proto\022\ngoogle.api\032\025google/"
"api/http.proto\032 google/protobuf/descript"
"or.proto:E\n\004http\022\036.google.protobuf.Metho"
"dOptions\030\260\312\274\" \001(\0132\024.google.api.HttpRuleB"
"n\n\016com.google.apiB\020AnnotationsProtoP\001ZAg"
"oogle.golang.org/genproto/googleapis/api"
"/annotations;annotations\242\002\004GAPIb\006proto3"
;
static const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable* const descriptor_table_annotations_2eproto_deps[2] = {
  &::descriptor_table_google_2fapi_2fhttp_2eproto,
  &::descriptor_table_google_2fprotobuf_2fdescriptor_2eproto,
};

const char descriptor_table_protodef_annotations_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
"\n\021annotations.proto\022\ngoogle.api\032\nhttp.pr"
"oto\032 google/protobuf/descriptor.proto:E\n"
"\004http\022\036.google.protobuf.MethodOptions\030\260\312"
"\274\" \001(\0132\024.google.api.HttpRuleBn\n\016com.goog"
"le.apiB\020AnnotationsProtoP\001ZAgoogle.golan"
"g.org/genproto/googleapis/api/annotation"
"s;annotations\242\002\004GAPIb\006proto3"
;
static const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable* const descriptor_table_annotations_2eproto_deps[2] = {
  &::descriptor_table_google_2fprotobuf_2fdescriptor_2eproto,
  &::descriptor_table_http_2eproto,
};
*/
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
