#include "protocol.h"

Protocol::Protocol()
{

}

std::string Protocol::typeRemover(std::string message)
{
    return message.substr(1, message.length());
}

std::string Protocol::userListClientMessageCreate()
{
    char firstSymbol = USER_LIST;
    std::string result = "";
    result.push_back(firstSymbol);
    return result;
}


// It is good idea to use space symbol for dividing users into our message,
// so reading will be much more simpler.
// Of course, names should not contain spaces inside themselfs.

std::string Protocol::userListServerMessageCreate(StringBufferPtr names)
{
    char firstSymbol = USER_LIST;
    std::string result = "";
    result.push_back(firstSymbol);

    for (std::string name : *names)
    {
        result += name;
        result += " ";
    }
    return (result.substr(0, result.length()-1));
}

StringSetPtr Protocol::userListServerMessageParse(std::string message)
{
    StringSetPtr result;
    std::stringstream tempStream(message.substr(1, message.length()));
    std::string tempString;
    while(tempStream >> tempString)
    {
        result->insert(tempString);
    }
    return result;
}



std::string Protocol::chatMessageClientMessageCreate(std::string user, std::string message)
{
    char firstSymbol = MESSAGE;
    std::stringstream tempStream;
    tempStream << firstSymbol << user << ": " << message;
    return tempStream.str();
}

std::string Protocol::chatMessageClientMessageParse(std::string message)
{
    return message.substr(1, message.length());
}

std::string Protocol::logInClientMessageCreate(std::string name)
{
    return typeAdder(LOG_IN, name);
}

std::string Protocol::logInServerMessageCreate(std::string status)
{
    return typeAdder(LOG_IN, status);
}

std::string Protocol::disconnectClientMessageCreate()
{
    return typeAdder(USER_DISCONNECT, "");
}

std::string Protocol::typeAdder(uint8_t type, std::string message)
{
    char firstSymbol = type;
    std::string result = "";
    result.push_back(firstSymbol);
    result += message;
    return result;
}

