#pragma once
#if !defined(AUTHENTICATE)
#define AUTHENTICATE

#include <map>
#include <set>
#include <fstream>
#include <iostream>
#include "simpleJSON.hpp"
#include "utility.hpp"
#include "logger.hpp"
#include "./../models/user.hpp"

class Authenticate{

    private:
    Logger logger;
    json::JSON authData;
    std::fstream authFile;

    public:
    Authenticate(){
        std::string tempAuthData = "", thisLine;
        authFile.open("./app/auth.json", std::ios_base::in);
        if(!authFile){
            logger.logError("Failed to open auth.json");
            exit(EXIT_FAILURE);
        }
        while (std::getline(authFile, thisLine)){
            tempAuthData.append(thisLine);
        }
        authData = json::JSON::Load(tempAuthData);
        authFile.close();
    }

    ~Authenticate(){
        authFile.open("./app/auth.json", std::ios_base::out);
        authFile<<authData;
        authFile.close();
    }

    User login(std::string username, std::string password, std::string database){
        User response(false, username, database);
        for(int i=0; i<authData.size(); i++){
            if(authData[i]["username"].ToString() == username && authData[i]["password"].ToString() == password){
                for(int j=0; j<authData[i]["database"].size(); j++){
                    if(authData[i]["database"][j].ToString() == database){
                        response.loginStatus = true;
                    }
                }
            }
        }
        return response;
    }
    
};

#endif