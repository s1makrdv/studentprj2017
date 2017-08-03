#include "DataBaseManager.h"
#include <thread>
#include "Helper.h"

std::map<std::string, ConnectionPtr> DataBaseManager::connections_;

bool DataBaseManager::getUsersList(std::vector<User> &users)
{
    ConnectionPtr connection = getConnection();
    bool is_success = true;

    try
    {
        pqxx::work txn(*connection);
        pqxx::result result = txn.exec("SELECT id, name, nick FROM users;");

        for (const pqxx::tuple& row : result)
        {
            User user;
            Helper::parseFromPostgres(row, user);
            users.push_back(user);
        }
        txn.commit();
    }
    catch(const std::exception& e)
    {
        LOG_INFO("Failure get user list " << e.what());
        is_success= false;
    }

    return is_success;
}

bool DataBaseManager::userExists(std::string name)
{
    ConnectionPtr connection = getConnection();
    bool exists = true;

    try
    {
        pqxx::work txn(*connection);
        pqxx::result result = txn.exec("SELECT name FROM users WHERE name = '" + name + "'");
        txn.commit();
        exists = !result.empty();
    }
    catch(const std::exception& e)
    {
        LOG_INFO("Failure check user exists: " << e.what());
    }
    return exists;
}

bool DataBaseManager::authUser(std::string name, std::string pass)
{
    ConnectionPtr connection = getConnection();
    bool exists = false;

    try
    {
        pqxx::work txn(*connection);
        pqxx::result result = txn.exec("SELECT name FROM users WHERE"
                                       " (name = '" + name + "') "
                                       "and (pass = '" + pass + "')");
        txn.commit();
        exists = !result.empty();
    }
    catch(const std::exception& e)
    {
        LOG_INFO("Failure check user: " << e.what());
    }
    return exists;
}

bool DataBaseManager::addUser(std::string name, std::string pass)
{
    ConnectionPtr connection = getConnection();
    bool is_success = true;

    try
    {
        pqxx::work txn(*connection);
        txn.exec("INSERT INTO users (id, name, pass)"
                                       "VALUES (DEFAULT, '" + name + "', '" + pass +"')"
                                       "RETURNING id");
        txn.commit();

    }
    catch(const std::exception& e)
    {
        LOG_INFO("Failure add user: " << e.what());
        is_success = false;
    }

    return is_success;
}

bool DataBaseManager::addChat(int& chatId, std::string name)
{
    ConnectionPtr connection = getConnection();
    bool is_success = true;
    bool exist = false;

    try
    {
        pqxx::work txn(*connection);
        pqxx::result result = txn.exec("SELECT id FROM chats WHERE name = '" + name + "'");
        txn.commit();

        exist = !result.empty();

        if(exist)
        {
            chatId = result[0][0].as<int>();
            LOG_INFO("Chat" << chatId << "allready exists");
        }
    }
    catch(const std::exception& e)
    {
        LOG_INFO("Failure select chat by name: " << e.what());
        is_success = false;
    }

    if(!exist)
    {
        try
        {
            pqxx::work txn(*connection);
            pqxx::result result = txn.exec("INSERT INTO chats (id,name)"
                                           " VALUES (DEFAULT,'" + name +"')"
                                           " RETURNING id;");
            txn.commit();

            chatId = result[0][0].as<int>();
            LOG_INFO("Create new chat: " << chatId );


        }
        catch(const std::exception& e)
        {
            LOG_INFO("Failure add chat: " << e.what());
            is_success= false;
        }
    }

    return is_success;
}

bool DataBaseManager::usersByChats(int chatId, std::string userName)
{
    ConnectionPtr connection = getConnection();
    bool is_success = true;
    bool exist = false;

    std::string chatId_ = std::to_string(chatId);
    try
    {
        pqxx::work txn(*connection);
        pqxx::result result = txn.exec("SELECT chat_id, user_id FROM users_by_chats"
                                       " WHERE chat_id = '" + chatId_ + "'"
                                         " and user_id = (SELECT id FROM users WHERE name = '" + userName + "') ");
        txn.commit();
        exist = !result.empty();

    }
    catch(const std::exception& e)
    {
        LOG_INFO("Failure add users_by_chats: " << e.what());
        is_success = false;
    }

    if(!exist)
    {
        try
        {
            pqxx::work txn(*connection);
            txn.exec("INSERT INTO users_by_chats (chat_id, user_id)"
                     "VALUES('" + chatId_ + "',"
                     "(SELECT id FROM users WHERE name = '" + userName + "'))");
            txn.commit();
        }
        catch(const std::exception& e)
        {
            LOG_INFO("Failure add users_by_chats: " << e.what());
            is_success = false;
        }
    }

    return is_success;
}


bool DataBaseManager::addMessage(int chatId, std::string userName
                                 , std::string message)
{
    ConnectionPtr connection = getConnection();
    bool is_success = true;

    std::string chatId_ = std::to_string(chatId);

    try
    {
        pqxx::work txn(*connection);
        txn.exec("INSERT INTO messages (id, chat_id, user_id, message)"
                 "VALUES (DEFAULT,'" + chatId_ +"',"
                 "(SELECT id FROM users WHERE "
                 "name = '" + userName + "'),'" + message + "')");

        txn.commit();
    }
    catch(const std::exception& e)
    {
        LOG_INFO("Failure add message: " << e.what());
        is_success= false;
    }

    return is_success;
}

bool DataBaseManager::getMessageList(std::string name, std::vector<ChatMessage> &chatMessages)
{
    ConnectionPtr connection = getConnection();
    bool is_success = true;

    try
    {
        pqxx::work txn(*connection);
        pqxx::result result = txn.exec(" SELECT messages.id, users_by_chats.chat_id, messages.user_id, message "
                                       " FROM users_by_chats inner JOIN messages ON users_by_chats.chat_id = messages.chat_id "
                                       " WHERE users_by_chats.user_id = (SELECT id from users where name = '"+name+"')");

        txn.commit();

        for (const pqxx::tuple& row : result)
        {
            ChatMessage chatMessage;
            Helper::parseChatMessages(row, chatMessage);
            chatMessages.push_back(chatMessage);

        }
    }
    catch(const std::exception& e)
    {
        LOG_INFO("Failure get Message list: " << e.what());
        is_success = false;
    }

    return is_success;
}

bool DataBaseManager::getChatsList(std::string& message)
{
    ConnectionPtr connection = getConnection();
    bool is_success = true;

    try
    {
        pqxx::work txn(*connection);
        pqxx::result result = txn.exec(" SELECT id, name FROM chats;");

        txn.commit();
        message.erase();
        for (const pqxx::tuple& row : result)
        {
              message += "CHATROOM:[" + row[0].as<std::string>() + "] "
                      + "Name: [" + row[1].as<std::string>() + "]\n";
        }
    }
    catch(const std::exception& e)
    {
        LOG_INFO("Failure get Chats list: " << e.what());
        is_success = false;
    }

    return is_success;
}

ConnectionPtr DataBaseManager::getConnection()
{
    std::stringstream ss;
    ss << std::this_thread::get_id();
    std::string thread_key = ss.str();

    LOG_INFO(thread_key);
    ConnectionPtr connection;

    if (connections_ .find(thread_key) == connections_.end())
    {
        try
        {
            connection.reset(new pqxx::connection(
                                 "user= 'admin'"
                                 "host= '127.0.0.1'"
                                 "password='admin'"
                                 "dbname='postgres'"
                                 "port = '5432'"));
        }
        catch (const std::exception& e)
        {
            LOG_ERR("Database connection failure" << e.what());
        }

        connections_[thread_key] = connection;

        LOG_INFO("Created connection for thread: " << thread_key);
    }
    else
    {
        connection = connections_[thread_key];
    }

    return connection;
}