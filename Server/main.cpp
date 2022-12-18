#include <iostream>
#include <vector>
#include <winsock2.h>
#include <typeinfo>
#include "services/authenticate.hpp"
#include "services/utility.hpp"
#include "services/query.hpp"
#include "services/logger.hpp"
#include "services/simpleJSON.hpp"
#include "./models/user.hpp"

using namespace std;

void handleExit(){
    Logger logger;
    logger.logStartup("Server Stopped.");
}

int main(){
    
    atexit(handleExit);

    /*
        Read data from config file. 
        1. crate a server
        2. Accept connection request
        3. authenticate user
        4. read query
        5. process query
        6. encode response into json and send to client.
    */

    Utility utility;
    json::JSON config = json::JSON::Load(utility.configData());
    SOCKET serverSocket, clientSocket;
    sockaddr_in serverAddress;
                serverAddress.sin_family = AF_INET;
                serverAddress.sin_port = htons(config["port"].ToInt());
                serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    sockaddr_in clientAddress;
    fd_set fileDesciptorReadMaster, fileDesciptorRead;
    timeval timevalue;
            timevalue.tv_sec = 120;
            timevalue.tv_usec = 0;
    WSADATA windowsSocketData;
    Logger logger;
    Authenticate auth;
    Query query;
    map<int, User> activeUsers;
    const int MAX_QUERY_LENGTH = config["maxQueryLength"].ToInt();


    /* 
        WSADATA is used to store socket information on windows computer.
        WSAStartup is used to initiate socket structure and information
     */
    if(WSAStartup(MAKEWORD(2,2), &windowsSocketData) == SOCKET_ERROR){
        logger.logError("Failed to initialize Windows Socket Data.");
        logger.logStartup("Server crashed. ERROR CODE: "+WSAGetLastError());
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    /*
        Intialiaze the server socket. We will initialise it with  
        1. Address family as internet
        2. Socket type as stream
        3. Protocol as TCP
    */
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(serverSocket == SOCKET_ERROR){
        logger.logError("Failed to initialize Socket Descriptor.");
        logger.logStartup("Server crashed. ERROR CODE: "+WSAGetLastError());
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    /*
        Bind the socket to local port to reserve the port and utilise it
    */
    if(bind(serverSocket, (sockaddr*)&serverAddress, sizeof(sockaddr)) == SOCKET_ERROR){
        logger.logError("Failed to Bind Socket to Local Port.");
        logger.logStartup("Server crashed. ERROR CODE: "+WSAGetLastError());
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    
    /*
        Start Listening to the port for any data or connection requet
    */
    if(listen(serverSocket, SOMAXCONN) == SOCKET_ERROR){
        logger.logError("Failed to Listen data from socket.");
        logger.logStartup("Server crashed. ERROR CODE: "+WSAGetLastError());
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    /*
        Server has been started successfully.
    */
    logger.logStartup("Server Started.");

    FD_ZERO(&fileDesciptorReadMaster);
    FD_SET(serverSocket, &fileDesciptorReadMaster);

    while(true){
        fileDesciptorRead = fileDesciptorReadMaster;
        int selectResponse = select(serverSocket, &fileDesciptorRead, NULL, NULL, &timevalue);
        for(int i=0; i<selectResponse; i++){
            SOCKET thisSocket = fileDesciptorRead.fd_array[i];
            if(thisSocket == serverSocket){
                /*
                    Accept Socket Connection request
                    Ask for authentication if auth is successfull add to aciveclients
                */
                int SOCKADDR_LENGTH = sizeof(sockaddr);
                clientSocket = accept(serverSocket, (sockaddr*)&clientAddress, &SOCKADDR_LENGTH);

                try{
                    logger.logInfo("Connected Client ID: "+to_string(thisSocket)+" IP: "+inet_ntoa(clientAddress.sin_addr));
                }catch(...){}

                if(clientSocket == SOCKET_ERROR){
                    /*
                     *  Failed to accept socket connection
                     */
                    logger.logError("Failed to accept connection request");
                }else{
                    char authRequestData[MAX_QUERY_LENGTH] = {0};
                    recv(clientSocket, authRequestData, MAX_QUERY_LENGTH, 0);
                    try{
                        json::JSON authJSON = json::JSON::Load(authRequestData);
                        User tempUser = auth.login(authJSON["username"].ToString(), authJSON["password"].ToString(), authJSON["database"].ToString());
                        if(tempUser.loginStatus == true){
                            /* 
                             *  Login was successfull
                             */
                            cout<<"User Authenticated Successfully, User: "<<tempUser.username<<endl;
                            FD_SET(clientSocket, &fileDesciptorReadMaster);
                            activeUsers.insert({clientSocket, tempUser});
                        }else{
                            /* 
                             *  Close client Socket because Authentication has failed.
                             */
                            cout<<"User Auth Failed. User was: "<<tempUser.username<<endl;
                            closesocket(clientSocket);
                        }
                    }catch(...){
                        /*
                         *  Exception parsing JSON
                         */
                        std::string parseError = "Failed to Parse Auth Query, Query: ";
                        parseError += authRequestData;
                        logger.logQueryError(parseError);
                        closesocket(clientSocket);
                    }
                }

            }else{
                /* A Client is sending some data. */
                char clientQuery[MAX_QUERY_LENGTH] = {0};
                int bytesIn = recv(thisSocket, clientQuery, MAX_QUERY_LENGTH, 0);
                if(bytesIn <= 0){
                    /*  
                        Either the client has closed the connection or some error has occured, 
                        closing client socket connection.
                    */
                    if(bytesIn == SOCKET_ERROR){
                        logger.logError("Error Reciving message from client "+to_string(thisSocket)+", Error: "+to_string(WSAGetLastError()));
                    }else{
                        cout<<"User Disconnected, User was: "<<activeUsers[thisSocket].username<<endl;
                    }
                    closesocket(thisSocket);
                    FD_CLR(thisSocket, &fileDesciptorReadMaster);
                    FD_CLR(thisSocket, &fileDesciptorRead);
                    activeUsers.erase(thisSocket);
                }else if(bytesIn > 2){
                    std::string queryResponse = query.process(clientQuery, activeUsers[thisSocket].database).to_json_string();
                    send(thisSocket, queryResponse.c_str(), queryResponse.length(), 0);
                }
            }
        }

    }

    return 0;
}