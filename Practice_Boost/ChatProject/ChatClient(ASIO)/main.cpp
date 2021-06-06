#include<boost/asio.hpp>
#include<boost/bind.hpp>
#include<memory>>
#include"ChatClient.h"

using namespace boost;


int main() {


    std::unique_ptr<CChatClient> uPtrChatClient = std::make_unique<CChatClient>();

    uPtrChatClient->Run();




}