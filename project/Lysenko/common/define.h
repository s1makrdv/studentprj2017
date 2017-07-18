#ifndef DEFINE
#define DEFINE

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#define WORKER_THREAD_COUNT 4
#define BUFFER_MAX_SIZE 65535

#define LOG_INFO(message) std::cout << __FILE__ << " : " << __FUNCTION__ << " : " << __LINE__ << " [INF] " << message << std::endl;
#define LOG_ERR(message) std::cout << __FILE__ << " : " << __FUNCTION__ << " : " << __LINE__ << " [ERR] " << message << std::endl;

enum class Operations : uint16_t
{
    logIn = 0
};

typedef std::vector<char> ByteBuffer;
typedef std::shared_ptr<ByteBuffer> ByteBufferPtr;
typedef std::vector<ByteBufferPtr> BuffersVector;

std::ostream& operator<<(std::ostream& stream, const ByteBuffer& buffer);
std::ostream& operator<<(std::ostream& stream, const BuffersVector& buffersVector);

#endif // DEFINE
