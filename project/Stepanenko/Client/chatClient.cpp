#include "chatClient.h"
#include "protocol.h"

ChatClient::ChatClient(std::string address, std::string port)
    : Client(address, port)
    , name_("")
    , loggedIn_(false)
    , inChat_(false)
{

}

void ChatClient::askNameAndRegister()
{
    while (name_ == "")
    {
        LOG_INFO("Please enter your name: ");
        std::cin >> name_;
        if (userNames_->find(name_) != userNames_->end())
        {
            LOG_INFO("Your name should be unique! This name is already used by other person.");
            name_="";
        }
        else
        {
            break;
        }
    }

    std::string registerMessage = Protocol::logInClientMessageCreate(name_);
    write(registerMessage);
}

void ChatClient::getUsersListFromServer()
{
    std::string message = Protocol::userListClientMessageCreate();
    write(message);
}

void ChatClient::printUsersToConsole()
{
    if (userNames_->size() == 0)
    {
        std::cout << "Server still has no one registered user" << std::endl;
    }
    else
    {
        std::cout << "Server has next registered users:" << std::endl;
        for (std::string user : *userNames_)
        {
            std::cout << user << ", ";
        }
        std::cout << std::endl;
    }
}

void ChatClient::sendChatMessage(std::string message)
{
    std::string messageToSend = Protocol::chatMessageClientMessageCreate(name_, message);
    write(messageToSend);
}

void ChatClient::connectToUser(std::string user)
{
    std::string message = Protocol::logInClientMessageCreate(user);
    write(message);
}

void ChatClient::disconnect()
{
    std::string message = Protocol::disconnectClientMessageCreate();
    write(message);
}

bool ChatClient::isInChat()
{
    return inChat_;
}

bool ChatClient::isLoggedIn()
{
    return loggedIn_;
}

void ChatClient::processInputMessage()
{
    int messageType = static_cast<int>(buffer_[0]);
    std::string message(buffer_.begin(), buffer_.end());
    switch (messageType)
    {
        case Protocol::USER_LIST:
        {
            userNames_ = Protocol::userListServerMessageParse(message);
            printUsersToConsole();
            break;
        }
        case Protocol::MESSAGE:
        {
            std::string userMessage = Protocol::chatMessageClientMessageParse(message);
            std::cout << userMessage << std::endl;
            break;
        }
        case Protocol::LOG_IN:
        {
            std::string status = Protocol::typeRemover(message);
            if (status == "OK")
            {
                loggedIn_ = true;
                std::cout << "You logged in successfuly!" << std::endl;
            }
            else
            {
                std::cout << "Your login is failed!" << std::endl;
                askNameAndRegister();
            }
            break;
        }
        case Protocol::START_CHAT:
        {
            std::string status = Protocol::typeRemover(message);
            if (status == "OK")
            {
                inChat_ = true;
                std::cout << "Your chat with remote user just has begun!" << std::endl;
            }
            else
            {
                std::cout << "You logged in successfuly!" << std::endl;
            }
            break;
        }
        case Protocol::USER_DISCONNECT: //This is disconnecting for my username
        {
            std::string status = Protocol::typeRemover(message);
            if (status == "OK")
            {
                loggedIn_ = false;
                inChat_ = false;
                std::cout << "You logged out successfuly!" << std::endl;
            }
            else
            {
                std::cout << "Your logout is failed! Try again later." << std::endl;
            }
            break;
        }
        default:
        {
            LOG_ERR("Type of the message is unknown: " << messageType);
        }

    }
}

