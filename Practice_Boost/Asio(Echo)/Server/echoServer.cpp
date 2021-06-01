#include<boost/asio.hpp>
#include<iostream>

constexpr char SERVER_IP[] = "127.0.0.1";
constexpr unsigned short PORT_NUMBER = 9000;


int main() {

    // Ŀ�ο��� �߻��� I/O �̺�Ʈ�� ����ġ ���ִ� Class
    // �ش� Ŭ������ ���� Ŀ�ο��� �߻��� ��Ʈ��ũ���� ���� �ޱ�, �����ϱ�, ������ �ޱ�, ������ ������ �̺�Ʈ�� �� �� �ִ�.
    boost::asio::io_service io_service;

    boost::asio::ip::tcp::endpoint endpoint{ boost::asio::ip::tcp::v4(),PORT_NUMBER };

    //Ŭ���̾�Ʈ�� ������ �޴� Ŭ����
    boost::asio::ip::tcp::acceptor acceptr{ io_service,endpoint };

    //TCP ���� ���� ���� �޽����� �ְ� �޾ƾ� �ϱ� ������ Io_server�� �Ҵ�
    boost::asio::ip::tcp::socket socket(io_service);

    //acceptor�� Ŭ���̾�Ʈ�� socket ����
    acceptr.accept(socket);

    std::cout << "Ŭ���̾�Ʈ ����\n";

    while (true) {

        //���ۼ���
        std::array<char, 128> buf{};

        //�ý��ۿ��� �߻��ϴ� ���� �ڵ带 ������ Ŭ����
        boost::system::error_code error;

        // Ŭ���̾�Ʈ�� ���� �����͸� �޴´�.(���� ũ�� ��ȯ)
        // Ŭ���̾�Ʈ ������ �������� ��쵵 Ȯ�ΰ���
        size_t len = socket.read_some(boost::asio::buffer(buf), error);


        if (error) {

            //������ �������� �� eof ���� �߻�
            if (error == boost::asio::error::eof) {
                std::cout << "Ŭ���̾�Ʈ�� ������ ���������ϴ�.\n";
            }
            else {
                std::cout << "error No: " << error.value() << "Error Message: " << error.message() << "\n";

            }
            break;

        }
        std::cout << "Ŭ���̾�Ʈ���� ���� �޽���:" << &buf[0] << "\n";

        //���� ����
        char szMessage[128]{};
        sprintf_s(szMessage, 128 - 1, "Re:%s", &buf[0]);
        int mMsgLen = strnlen_s(szMessage, 128 - 1);

        boost::system::error_code ignored_error;
        
        //  �޽����� ���� �� ���
        //  ���� �����ͷ��� ������ �� ����
        socket.write_some(boost::asio::buffer(szMessage, mMsgLen), ignored_error);
        std::cout << "Ŭ���̾�Ʈ�� ���� �޽���" << szMessage << "\n";

    }
    getchar();

    return 0;

}