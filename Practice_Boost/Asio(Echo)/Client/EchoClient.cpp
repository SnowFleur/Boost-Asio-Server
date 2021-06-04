#include<boost/asio.hpp>
#include<iostream>

constexpr char SERVER_IP[] = "127.0.0.1";
constexpr unsigned short PORT_NUMBER = 9000;


int main() {

    boost::asio::io_service io_service;

    //from_string ���·� ��ȯ
    boost::asio::ip::tcp::endpoint endpoint{ boost::asio::ip::address::from_string(SERVER_IP),PORT_NUMBER };

    boost::system::error_code connect_error;

    boost::asio::ip::tcp::socket socket(io_service);
    
    socket.connect(endpoint, connect_error);

    if (connect_error) {
        std::cout << "���� ���� error No:" << connect_error.value() << "Message: " << connect_error.message() << "\n";
    
        getchar();
        return 0;
    }
    else {
        std::cout << "������ ���� ����\n";


        for (int i = 0; i < 7; ++i) {
            char szMessage[128]{};
            sprintf_s(szMessage, 128 - 1, "%d- Send Message", i);
            int nMsgLen = strnlen_s(szMessage, 128 - 1);

            boost::system::error_code ignored_error;
            socket.write_some(boost::asio::buffer(szMessage, nMsgLen), ignored_error);

            std::cout << "������ ���� �޽���:" << szMessage << "\n";

            std::array<char, 128> buf{};

            boost::system::error_code error;

            size_t len = socket.read_some(boost::asio::buffer(buf), error);


            if (error) {

                if (error == boost::asio::error::eof) {
                    std::cout << "������ ������ ���������ϴ�.\n";
                }
                else {
                    std::cout << "���� ���� error No:" << error.value() << "Message: " << error.message() << "\n";
                }
                break;
            }

            std::cout << "�����κ��� ���� �޽���: " << &buf[0] << "\n";

        }

        // ��Ʈ��ũ ��������̸� true 
        if (socket.is_open()) {
            socket.close();
        }

        getchar();
        return 0;

    }


}