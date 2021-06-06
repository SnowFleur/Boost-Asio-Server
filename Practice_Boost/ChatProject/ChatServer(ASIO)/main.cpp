#include"ChatServer.h"
#include<memory>


//#include"../protocol_generated.h"
//
//using namespace ChatServerProtocol;
//
//flatbuffers::FlatBufferBuilder builder;
//
////데이터 직렬화
//auto packet = Createsc_loginOk_packet(builder, sizeof(sc_loginOk_packet));
//builder.Finish(packet);
////데이터 역직렬화

int main() {

    std::unique_ptr<CChatServer> uPtrChatServer = std::make_unique<CChatServer>();


    uPtrChatServer->Run();

}