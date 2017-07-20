#ifndef HELPER_H
#define HELPER_H

#include "define.h"

class Helper
{
public:
    Helper() = delete;
    static ByteBufferPtr makeBuffer(ByteBufferPtr message);
    static uint16_t mergeTwoByte(uint8_t high, uint8_t low);
    static void insertCommandCode(ByteBufferPtr buffPtr, CommandCode code);
    static ByteBuffer stringToBuffer(const std::string& str);
    static std::string bufferToString(ByteBufferPtr buffPtr, uint posFrom);
};

#endif // HELPER_H
