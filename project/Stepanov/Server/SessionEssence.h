#ifndef SESSIONESSENCE_H
#define SESSIONESSENCE_H

#include "Session.h"
#include "Define.h"



class SessionEssence : public Session
{
public:
    SessionEssence();

    static std::shared_ptr<SessionEssence> getNewSession();

    char getIdClient();

    std::string message_;

    std::string getLogin() const;

    std::string getTargetLogin() const;

    void setHasRequest(bool value);

    void setInChat(bool value);

private:
    void onRead(ByteBuffer data) override;
    std::string login;
    std::string targetLogin;
    int currentRoom;
    std::vector<int> availableRooms;
    bool hasRequest;
    bool inChat;
};

typedef std::shared_ptr<SessionEssence> SessionEssencePtr;

#endif // SESSIONESSENCE_H
