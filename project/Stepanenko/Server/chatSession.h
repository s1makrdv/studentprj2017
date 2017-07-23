#ifndef CHATSESSION_H
#define CHATSESSION_H

#include "sesion.h"

class ChatSession : public Session
{
public:
    ChatSession();
    static std::shared_ptr<ChatSession> getNewSession();
    void onRead(ByteBuffer data) override;
    void subscribe(std::function<void(std::shared_ptr<ChatSession>, std::string)> cb);
private:
    std::vector<std::function<void(std::shared_ptr<ChatSession>, std::string)>> onReadCbs;
};
typedef std::shared_ptr<ChatSession> ChatSessionPtr;
#endif // CHATSESSION_H