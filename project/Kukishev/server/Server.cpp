#include "Server.h"

#include "define.h"
#include "Worker.h"

Server::Server(int port)
    : io_service_(Worker::instance()->io_service())
    , acceptor_( io_service_
          , asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))

{

}

void Server::startAccept()
{
    SessionPtr session = Session::getNewSessions();

    acceptor_.async_accept(session->socket(), [this, session](asio::error_code error)
    {
        if(!error)
        {
            sessions_.push_back(session);
            session->start();
        }

        startAccept();
    });
}

