#pragma once
#if !defined(QUERY)
#define QUERY

#include <iostream>
#include <list>
#include <map>
#include <time.h>
#include <vector>
#include <fstream>
#include "utility.hpp"
#include "logger.hpp"
#include "simpleJSON.hpp"
#include "./../models/queryResponse.hpp"

class Query{

    Utility utility;
    Logger logger;

    public:
        QueryResponse process(std::string query, std::string database){

            QueryResponse queryResponse;
            clock_t startTime = clock();

            try{
                json::JSON object = json::JSON::Load(query);
                std::string operation = object["operation"].ToString();
                std::string collection = object["collection"].ToString();
                operation =  utility.convertToLowerCase(operation);

                if(operation == "add"){
                    try{
                        setupCollectionPath(database, collection);
                        const std::string documentID = utility.generateUniqueID()+".json";
                        const std::string documentPath = "./data/"+database+"/"+collection+"/"+documentID;
                        std::fstream tempFile;
                        tempFile.open(documentPath, std::ios::out);
                        tempFile<<object["data"];
                        tempFile.close();
                        queryResponse.documentWrites++;
                        queryResponse.hasError = false;
                        queryResponse.message = documentID+" inserted Successfully.";
                    }catch(...){
                        logger.logQueryError("Failed to Add Document to collection "+collection+" of database "+database);
                        queryResponse.hasError = true;
                        queryResponse.message = "Error while adding Document to the collection "+collection+" of database "+database;
                    }
                }else if(operation == "find"){
                    try{
                        setupCollectionPath(database, collection);
                        std::list<std::string> documentPathList = getDocumentList(database, collection);
                        queryResponse.data = filterAndRead(documentPathList, object["filter"]);
                        queryResponse.documentReads = documentPathList.size();
                        queryResponse.hasError = false;
                        queryResponse.message = "Got Document list successfully.";
                    }catch(...){
                        logger.logQueryError("Failed to Find Document(s) from collection "+collection+" of database "+database);
                        queryResponse.hasError = true;
                        queryResponse.message = "Error while finding Document in the collection "+collection+" of database "+database;
                    }
                }else if(operation == "update"){
                    try{
                        setupCollectionPath(database, collection);
                        std::list<std::string> documentPathList = getDocumentList(database, collection);
                        queryResponse.documentWrites = filterAndUpdate(documentPathList, object["filter"], object["data"]);
                        queryResponse.documentReads = documentPathList.size();
                        queryResponse.hasError = false;
                        queryResponse.message = "Document(s) updated successfully.";
                    }catch(...){
                        logger.logQueryError("Failed to Update Document in collection "+collection+" of database "+database);
                        queryResponse.hasError = true;
                        queryResponse.message = "Error while updating Document in the collection "+collection+" of database "+database;
                    }
                }else if(operation == "delete"){
                    try{
                        setupCollectionPath(database, collection);
                        std::list<std::string> documentPathList = getDocumentList(database, collection);
                        queryResponse.documentDeletes = filterAndDelete(documentPathList, object["filter"]);
                        queryResponse.documentReads = documentPathList.size();
                        queryResponse.hasError = false;
                        queryResponse.message = "Document(s) deleted successfully.";
                    }catch(...){
                        logger.logQueryError("Failed to delete Document(s) in collection "+collection+" of database "+database);
                        queryResponse.hasError = true;
                        queryResponse.message = "Error while deleting Document in the collection "+collection+" of database "+database;
                    }
                }

            }catch(...){
                logger.logQueryError("Failed to Parse Object, Object was: "+query);
                queryResponse.hasError = true;
                queryResponse.message = "Error Parsing Object.";
            }
            queryResponse.processTime = (float)(clock() - startTime)/CLOCKS_PER_SEC;
            return queryResponse;
        }


    private:

        std::list<std::string> getDocumentList(std::string database, std::string collection){
            std::list<std::string> documentList;
            try{
                DIR *dir;
                struct dirent *ent;
                const std::string collectionPath = "./data/"+database+"/"+collection;
                if ((dir = opendir (collectionPath.c_str())) != NULL) {
                    while ((ent = readdir (dir)) != NULL){
                        std::string filePath = "./data/"+database+"/"+collection+"/"+std::string(ent->d_name);
                        std::string extension = filePath.substr(filePath.size() - 4);
                        if(extension == "json"){
                            documentList.push_back(filePath);
                        }
                    }
                  closedir (dir);
                }
            }catch(...){
                logger.logError("Failed to get Document list in collection "+collection+"of database"+database);
            }
            return documentList;
        }

        std::list<json::JSON> filterAndRead(std::list<std::string> documentPathList, json::JSON filter){
            std::list<json::JSON> documentList;
            while (documentPathList.size() > 0){
                std::string documentPath = documentPathList.front();
                try{
                    json::JSON documentData = json::JSON::Load(readDocument(documentPath));
                    if(filterDocument(documentData, filter) == true){
                        documentList.push_back(documentData);
                    }
                }catch(...){
                    logger.logQueryError("Failed to read Document "+documentPath);
                }
                documentPathList.pop_front();
            }
            return documentList;
        }

        int filterAndDelete(std::list<std::string> documentPathList, json::JSON filter){
            int deleteCount = 0;
            while (documentPathList.size() > 0){
                std::string documentPath = documentPathList.front();
                try{
                    json::JSON documentData = json::JSON::Load(readDocument(documentPath));
                    if(filterDocument(documentData, filter) == true){
                        deleteCount++;
                        remove(documentPath.c_str());
                    }
                }catch(...){
                    logger.logQueryError("Failed to delete Document "+documentPath);
                }
                documentPathList.pop_front();
            }
            return deleteCount;
        }

        int filterAndUpdate(std::list<std::string> documentPathList, json::JSON filter, json::JSON newData){
            int updateCount = 0;
            while (documentPathList.size() > 0){
                std::string documentPath = documentPathList.front();
                try{
                    json::JSON documentData = json::JSON::Load(readDocument(documentPath));
                    if(filterDocument(documentData, filter) == true){
                        /**
                         * Update newData's value in DocumentData then put it on documentPath.
                        */
                        std::map<std::string, json::JSON> newDataAsMap(newData.getJsonAsMap());
                        std::map<std::string, json::JSON>::iterator itr;
                        for(itr = newDataAsMap.begin(); itr != newDataAsMap.end(); itr++){
                            documentData[itr->first] = itr->second;
                        }
                        std::ofstream document(documentPath, std::ofstream::trunc);
                        document<<documentData;
                        document.close();
                        updateCount++;
                    }
                }catch(...){
                    logger.logQueryError("Failed to Update Document "+documentPath);
                }
                documentPathList.pop_front();
            }
            return updateCount;
        }

        bool filterDocument(json::JSON document, json::JSON filter){
            bool doesMatchAllFilter = true;
            if(filter.size() > 0){
                std::map<std::string, json::JSON> filterAsMap(filter.getJsonAsMap());
                std::map<std::string, json::JSON>::iterator itr;
                for(itr = filterAsMap.begin(); itr != filterAsMap.end(); itr++){
                    if(document.hasKey(itr->first) && document[itr->first].getDataType() == filter[itr->first].getDataType()){
                        std::string dataType = filter[itr->first].getDataType();
                        if(dataType == "string" && filter[itr->first].ToString() != document[itr->first].ToString()){
                            doesMatchAllFilter = false;
                        }else if(dataType == "integral" && filter[itr->first].ToInt() != document[itr->first].ToInt()){
                            doesMatchAllFilter = false;
                        }else if(dataType == "floating" && filter[itr->first].ToFloat() != document[itr->first].ToFloat()){
                            doesMatchAllFilter = false;
                        }else if(dataType == "boolean" && filter[itr->first].ToBool() != document[itr->first].ToBool()){
                            doesMatchAllFilter = false;
                        }else if(dataType == "null" && (filter[itr->first].IsNull() == false || document[itr->first].IsNull() == false)){
                            doesMatchAllFilter = false;
                        }else if(dataType == "object"){
                            doesMatchAllFilter = false;
                        }else if(dataType == "array"){
                            doesMatchAllFilter = false;
                        }
                    }else{
                        doesMatchAllFilter = false;
                    } 
                }
            }
            return doesMatchAllFilter;
        }

        std::string readDocument(std::string documentPath){
            std::fstream documentData;
            documentData.open(documentPath, std::ios_base::in);
            std::string data = "", thisLine;
            while (getline(documentData, thisLine)){
                data.append(thisLine);
            }
            documentData.close();
            return data;
        }

        void setupCollectionPath(std::string database, std::string collection){
            std::string folder = "./data";
            /**
             * Create ./data folder
             */
            mkdir(folder.c_str());
            /**
             * Create ./data/database Folder
             */
            folder = "./data/"+database;
            mkdir(folder.c_str());

            /**
             * Create ./data/database/collection Folder
             */
            folder = "./data/"+database+"/"+collection;
            mkdir(folder.c_str());
        }

};

#endif