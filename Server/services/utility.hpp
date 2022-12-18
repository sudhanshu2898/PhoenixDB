#pragma once
#if !defined(UTILITY)
#define UTILITY

#include <vector>
#include <iostream>
#include <ctime>
#include <map>
#include <sstream>
#include <chrono>
#include <io.h>
#include <dirent.h>

class Utility{
    public:

        std::vector<std::string> splitString(std::string sentence, std::string delimiter){
            std::vector<std::string> wordList;
            size_t pos = 0;
            std::string token;
            while ((pos = sentence.find(delimiter)) != std::string::npos) {
                token = sentence.substr(0, pos);
                wordList.push_back(token);
                sentence.erase(0, pos + delimiter.length());
            }wordList.push_back(sentence);
            return wordList;
        }

        std::string configData(){
            std::fstream configFile;
            configFile.open("./app/config.json", std::ios_base::in);
            std::string config = "", thisLine;
            while (getline(configFile, thisLine)){
                config.append(thisLine);
            }
            configFile.close();
            return config;
        }

        std::string convertToLowerCase(std::string input){
            for(int i=0; i<input.length(); i++){
                input[i] = tolower(input[i]);
            }
            return input;
        }

        std::string generateUniqueID(){
            long int time = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            std::string response = std::to_string(time);
            response += "-";
            const char alphanum[] = "0123456789abcdefghijklmnopqrstuvwxyz";
            for (int i=0; i<15; i++){
                response += alphanum[rand() % (sizeof(alphanum) - 1)];
            }
            return response;
        }

};

#endif