#include "Server.h"

#include "define.h"
#include "Worker.h"

Server::Server(int port)
    : io_service_(Worker::instance()->io_service())
    , acceptor_( io_service_
          , asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
    //, socket_(io_service_)
{

}

void Server::start_accept()
{
    SessionPtr session = Session::getNewSession();
    acceptor_.async_accept(session->socket(), [session, this](asio::error_code error)
    {
        if (!error)
        {

            sessions_.push_back(session);
            session->start();
        }

        start_accept();
    });
}

