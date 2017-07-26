#ifndef HELPER_H
#define HELPER_H

#include <Define.h>
#include <Helper.h>
#include <pqxx/pqxx>


typedef std::vector<ByteBufferPtr> BuffersVector;
typedef std::vector<asio::const_buffer> EndBuffer;
class Helper
{
public:
    static BuffersVector addSize(ByteBufferPtr buffer);
    static EndBuffer makeEndBuffer(BuffersVector buffers);
    static uint16_t getSize(uint16_t left, uint16_t right);
    static bool paceFromPostgres(const pqxx::tuple& data, User& user);
};

#endif // HELPER_H