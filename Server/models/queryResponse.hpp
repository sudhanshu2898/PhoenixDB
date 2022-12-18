#pragma once
#if !defined(QUERYRESPONSE)
#define QUERYRESPONSE

#include <iostream>
#include <map>
#include <list>
#include <time.h>
#include "../services/simpleJSON.hpp"

class QueryResponse{
    public:
        bool hasError = false;
        std::string message = "NO ERROR";
        std::clock_t processTime = 0;
        std::list<json::JSON> data;
        unsigned long long documentCount = 0;
        unsigned long long documentReads = 0;
        unsigned long long documentWrites = 0;
        unsigned long long documentDeletes = 0;

        std::string to_json_string(){

            json::JSON responseData;
            responseData["hasError"] = this->hasError;
            responseData["message"] = this->message;
            responseData["processTime"] = this->processTime;
            responseData["data"] = json::Array();
            while(this->data.size() > 0){
                responseData["data"].append(this->data.front());
                this->data.pop_front();
                this->documentCount++;
            }
            responseData["documentCount"] = this->documentCount;
            responseData["documentReads"] = this->documentReads;
            responseData["documentWrites"] = this->documentWrites;
            responseData["documentDeletes"] = this->documentDeletes;
            
            return responseData.dump();
        }
};

#endif