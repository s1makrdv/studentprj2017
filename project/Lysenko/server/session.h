#ifndef SESSION_H
#define SESSION_H

#include <memory>

#include <asio.hpp>

#include "define.h"
#include "bufferconverter.h"

class Session;

typedef std::shared_ptr<Session> SessionPtr;

class Session
        : public std::enable_shared_from_this<Session>
{
    public:

        Session();

        static SessionPtr getNewSession();

        void start();
        void write(std::string message);

        asio::ip::tcp::socket& getSocket();

    private:

        void readMessageSize();
        void readMessage(uint16_t messageSize);
        void handleWrite(BuffersVector data,
                         asio::error_code error,
                         size_t writtenBytesCount);
        void handleReadMsgSize(asio::error_code error, size_t bufferSize);
        void handleReadMessage(asio::error_code error, size_t bufferSize);

        asio::ip::tcp::socket socket_;
        ByteBuffer buffer_;
};

#endif // SESSION_H