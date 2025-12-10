// Galaxy Wars by Maaz & Danyal

#pragma once
#include <exception>
#include <string>

class GameException : public std::exception

{


private:


    std::string message;

public:
    

    GameException(const std::string& msg) : message(msg) {}

    virtual const char* what() const throw() 
    
    {
        return message.c_str();
    }
};