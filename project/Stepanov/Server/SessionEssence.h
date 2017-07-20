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

    bool hasRequest;
    bool inChat;

    std::string message_;


private:
    void onRead(ByteBuffer data) override;
};

typedef std::shared_ptr<SessionEssence> SessionEssencePtr;

#endif // SESSIONESSENCE_H