#include "Helper.h"

void Helper::eraseCodeCommand(ByteBufferPtr bufferPtr)
{
    bufferPtr->erase(bufferPtr->begin());
}

std::string Helper::bufferToString(ByteBufferPtr bufferPtr)
{
    return std::string(bufferPtr->begin(), bufferPtr->end());
}

ByteBuffer Helper::stringToBuffer(const std::string &str)
{
    return ByteBuffer(str.begin(), str.end());
}

bool Helper::parseDromPostgres(const pqxx::tuple &data, User &user)
{
    user.id_ = data["id"].as<int>();
    user.name_ = data["name"].as<std::string>();
    user.nick_ = data["nick"].as<std::string>();

    LOG_INFO("Parsed user: " << user);

    return true;
}

void Helper::addCodeCommand(CodeCommand code, ByteBufferPtr bufferPtr)
{
   bufferPtr->emplace(bufferPtr->begin(), static_cast<uint8_t>(code));
}

BuffersVector Helper::addBufferSize(ByteBufferPtr buffer)
{
    BuffersVector result;

    ByteBufferPtr bufferSize_(new ByteBuffer());
    bufferSize_->resize(2);
    int size = buffer->size();

    (*bufferSize_)[0] = ((size & 0xff00) << 8);
    (*bufferSize_)[1] = (size & 0x00ff);
    result.push_back(bufferSize_);
    result.push_back(buffer);

    return result;
}

BufferSequence Helper::toBufferSequence(BuffersVector buffers)
{
    BufferSequence result;    
    for ( ByteBufferPtr buffer : buffers)
    {
        result.push_back(asio::buffer(*buffer));
    }
    return result;
}

std::vector<std::string> Helper::splitCommandAndData(const std::string &s)
{
    std::vector<std::string> tokens;
    tokens.push_back(s.substr(0,s.find(' ')));
    tokens.push_back(s.substr(s.find(' ')+1));
    return tokens;
}

std::pair<CodeCommand, ByteBufferPtr> Helper::getCodeAndData(const std::string &str)
{
    std::istringstream ist(str);

        std::string codeStr;
        ist >> codeStr;

        int code;

        try{
            code = std::stoi(codeStr);
        }
        catch(const std::invalid_argument& exp)
        {
            std::cout << "Wrong number of command!" << std::endl;
            return std::pair<CodeCommand, ByteBufferPtr>();
        }

        if(code < static_cast<int>(CodeCommand::LOGIN) || code > static_cast<int>(CodeCommand::ANSWER_ON_REQUEST))
        {
            std::cout << "Wrong number of command!" << std::endl;
            return std::pair<CodeCommand, ByteBufferPtr>();
        }


        CodeCommand commandCode = static_cast<CodeCommand>(code);

        std::string data;
        std::string s;
        ist >> s;
        data = s;
        while(ist >> s)
        {
            data+=" "+s;
        }
        ByteBufferPtr buff = std::make_shared<ByteBuffer>();

            if(!data.empty())
                buff = std::make_shared<ByteBuffer>(data.begin(), data.end());

        return std::make_pair(commandCode, buff);
}


