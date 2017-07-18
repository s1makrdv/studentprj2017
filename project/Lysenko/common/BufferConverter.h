#ifndef BUFFERCONVERTER_H
#define BUFFERCONVERTER_H

#include <vector>

#include <asio.hpp>

#include "define.h"

typedef std::vector<asio::const_buffer> WriteBuffer;

class BufferConverter
{
    public:

        static BuffersVector addMessageSize(ByteBufferPtr sourceMessage);
        static BuffersVector getOperationMessage(Operations operationCode);
        static ByteBuffer uint16ToBuffer(uint16_t number);
        static WriteBuffer toWriteBuffer(BuffersVector sourceBuffer);
        static uint16_t charsToMessageSize(ByteBuffer& sourceBuffer);
};

#endif // BUFFERCONVERTER_H