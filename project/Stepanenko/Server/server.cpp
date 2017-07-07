#include "server.h"

#include "define.h"
#include "worker.h"

Server::Server(int port)
    : io_service_(Worker::instance()->io_service())
    , acceptor_(io_service_
          , asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
    , socket_(io_service_)
{

}

void Server::start_accept()
{
    acceptor_.async_accept(socket_, [this](asio::error_code error)
    {
        if (!error)
        {
            SessionPtr session = Session::getNewSession(socket_);
            sessions_.push_back(session);
            session->start();
        }
        start_accept();
    });
}

