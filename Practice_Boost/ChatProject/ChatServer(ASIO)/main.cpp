#include"ChatServer.h"
#include<memory>


//#include"../protocol_generated.h"
//
//using namespace ChatServerProtocol;
//
//flatbuffers::FlatBufferBuilder builder;
//
////������ ����ȭ
//auto packet = Createsc_loginOk_packet(builder, sizeof(sc_loginOk_packet));
//builder.Finish(packet);
////������ ������ȭ

int main() {

    std::unique_ptr<CChatServer> uPtrChatServer = std::make_unique<CChatServer>();


    uPtrChatServer->Run();

}