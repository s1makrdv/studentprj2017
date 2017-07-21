#ifndef CHATMANAGER_H
#define CHATMANAGER_H

#include "Server.h"
#include "SessionManager.h"
#include "ChatRoom.h"

class ChatManager
{
public:



    static ChatManager &getInstance();

    void onConnected(SessionManagerPtr session);

    void getUserList(char idClient);

    void start(Server& server);

    void debug();

    void sendMessage(char idClient, char idTarget, std::string message);

    void sendChatMessage(char idRoom, std::string message, char idClient);

    void requestMessage(char idClient, char idTarget, std::string message);

    void createChat();

    void addUserToChatRoom(char idClient, char idRoom);

    std::string message_;

    std::vector<SessionManagerPtr> sessions_;

private:

    std::vector<ChatRoomPtr> chatRooms_;
    static ChatManager * p_instance;
    ChatManager();
    ChatManager(const ChatManager&);
    ChatManager& operator =(ChatManager&);

};

#endif // CHATMANAGER_H