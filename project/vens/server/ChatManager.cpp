#include "ChatManager.h"
#include "DataBaseManager.h"

ChatManager::ChatManager(Server &server)
{
    server.subscribe(std::bind(
                         &ChatManager::onConnected
                         , this
                         , std::placeholders::_1));
}

void ChatManager::onConnected(ChatSessionPtr session)
{
    LOG_INFO("Connected session.");
    sessions_.push_back(session);

    std::vector<User> users;
    DataBaseManager::getUsersList(users);
}
