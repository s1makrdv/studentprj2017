#include "SessionEssence.h"
#include "ChatManager.h"
SessionEssence::SessionEssence()
    :Session()
    , hasRequest(false)
{

}
ChatManager& c = ChatManager::getInstance();
std::shared_ptr<SessionEssence> SessionEssence::getNewSession()
{
    SessionEssencePtr session = std::make_shared<SessionEssence>();
    return session;
}

char SessionEssence::getIdClient()
{
    return idClient;
}

void SessionEssence::onRead(ByteBuffer data)
{
    std::string message(buffer_.begin(), buffer_.end());

    if(message.find(LOGIN_MESSAGE) != std::string::npos)
    {
        message.erase(message.begin(),message.begin()+2);
        login = message;
        if(c.authFunction(message))
        {
            message=LOGIN_INTO_ACCOUNT;
            message+="That name already registered!Please enter a password!";
            write(message);
        }
        else
        {
            message=CREATE_NEW_USER;
            message+="You a new user! Please enter a your password!";
            write(message);
        }
        LOG_INFO("Login is"<<login);
        /*idClient = message[2];
        LOG_INFO("Registered client with id = "<<idClient);
        write("You succesesfully registered!");*/

    }
    else if(message.find(LOGIN_INTO_ACCOUNT) != std::string::npos)
    {
        message.erase(message.begin(),message.begin()+2);
        LOG_INFO("Pass on server"<<message);
        if(c.loginIntoUser(login,message))
        {
            message="Welcome to chat " + login;
            write(message);
        }
        else
        {
            message=LOGIN_INTO_ACCOUNT;
            message+="Wrong password! Try again!";
            write(message);
        }

    }
    else if(message.find(CREATE_NEW_USER) != std::string::npos)
    {
        message.erase(message.begin(),message.begin()+2);
        c.registerNewUser(login ,message);
    }
    else if(message.find(GET_USER_LIST_MESSAGE) != std::string::npos)
    {
       // c.getUserList(idClient);
        c.getUserList(login);
    }
    else if(message.find(CREATE_CHAT_MESSAGE) != std::string::npos)
    {
        idTarget=message[2];
        message.erase(message.begin(),message.begin()+2);
        targetLogin=message;
        LOG_INFO("User "<<idClient<<" wish to create chat with " << idTarget<<" !");
        hasRequest=true;
        currentRoom=c.nextIdRoom;
        //c.requestMessage(idClient,idTarget,REQUEST_TO_CREATE_CHAT_MESSAGE,currentRoom);
        c.requestMessage(login,targetLogin,REQUEST_TO_CREATE_CHAT_MESSAGE,currentRoom);
        c.createChat();
        //c.addUserToChatRoom(idClient,currentRoom);
        c.addUserToChatRoom(login,currentRoom);

    }
    else if(message.find(DIRECT_MESSAGE) != std::string::npos)
    {
        idTarget=message[2];
        std::string send(message.begin()+3,message.end());
        LOG_INFO("message on server side"<<send);
        c.sendMessage(idClient,idTarget,send);
    }
    else if((message.find(YES_MESSAGE) != std::string::npos)&&(hasRequest))
    {
        currentRoom=message[2]-'0';
        //c.addUserToChatRoom(idClient,currentRoom);
        c.addUserToChatRoom(login,currentRoom);
    }
    else if(message.find(CHAT_MESSAGE) != std::string::npos)
    {
        //c.sendChatMessage(currentRoom,message,idClient);
        c.sendChatMessage(currentRoom,message,login);
    }
    else if(message.find(ADD_USER_TO_CHAT_MESSAGE) != std::string::npos)
    {
        idTarget=message[2];
        message.erase(message.begin()+2,message.end());
        targetLogin=message;
        //c.requestMessage(idClient,idTarget,REQUEST_TO_CREATE_CHAT_MESSAGE,currentRoom);
        c.requestMessage(login,targetLogin,REQUEST_TO_CREATE_CHAT_MESSAGE,currentRoom);
        LOG_INFO("Session trying to send request");
    }

}

std::string SessionEssence::getTargetLogin() const
{
    return targetLogin;
}

std::string SessionEssence::getLogin() const
{
    return login;
}