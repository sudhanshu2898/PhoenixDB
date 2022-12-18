#pragma once
#if !defined(USER)
#define USER

#include <iostream>
class User{
    public: 
        bool loginStatus;
        std::string username;
        std::string database;
        User(){}
        User(bool loginStatus, std::string username, std::string database){
            this->loginStatus = loginStatus;
            this->username = username;
            this->database = database;
        }
};

#endif