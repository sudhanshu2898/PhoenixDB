#pragma once
#if !defined(LOGGER)
#define LOGGER

#include<iostream>
#include<fstream>
#include<time.h>

class Logger{

    std::string formateMessage(std::string message){
        /** Get Current Time */
        time_t dateTimeNow = time(0);
        std::string dateTime = ctime(&dateTimeNow);
        dateTime[dateTime.length() - 1] = '\0';
        /** Fomate the message */
        message = dateTime+"\t"+message+"\n";
        return message;
    }

    public:

    void logInfo(std::string message){
        /* WRITE TO INFO LOG FILE */
        std::fstream logFile;
        logFile.open("./log/info.log", std::ios_base::app);
        logFile<<formateMessage(message);
        logFile.close();
    }

    void logError(std::string message){
        /* WRITE TO ERROR LOG FILE */
        std::fstream logFile;
        logFile.open("./log/error.log", std::ios_base::app);
        logFile<<formateMessage(message);
        logFile.close();
    }

    void logQueryError(std::string message){
        /* WRITE TO QUERY ERROR LOG FILE */
        std::fstream logFile;
        logFile.open("./log/queryError.log", std::ios_base::app);
        logFile<<formateMessage(message);
        logFile.close();
    }

    void logStartup(std::string message){
        /* WRITE TO STARTUP LOG FILE */
        std::fstream logFile;
        logFile.open("./log/startup.log", std::ios_base::app);
        logFile<<formateMessage(message);
        logFile.close();
    }
};

#endif