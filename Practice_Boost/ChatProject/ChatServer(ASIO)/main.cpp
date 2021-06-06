#include"ChatServer.h"
#include<memory>

int main() {

    std::unique_ptr<CChatServer> uPtrChatServer = std::make_unique<CChatServer>();


    uPtrChatServer->Run();



}