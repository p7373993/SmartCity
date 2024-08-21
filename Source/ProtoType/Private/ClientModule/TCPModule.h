#pragma once

#include "CoreMinimal.h"
#include "ProtoType/Global/Structs.h"
#include <vector>
#include <stdio.h>
#include <iostream>
#include <Winsock2.h>
#include <string.h>

//DeclarationTCPModule //--TCPModule& MyTCPModule = TCPModule::GetInstance();--//

class PROTOTYPE_API TCPModule
{
public:
    static TCPModule& GetInstance()
    {
        static TCPModule instance;
        return instance;
    }
    TCPModule(const TCPModule&) = delete;
    TCPModule& operator=(const TCPModule&) = delete;
    void TCPCunnect();
    std::vector<APData> GetAPData(std::vector<float> Elemental);

private:
    TCPModule() {};

    void HandleError(const char* cause);
    
    std::vector<APData> SAPData;
    SOCKET s;
    WSADATA wsaData;
    SOCKADDR_IN addr;
    char buffer[1024];
    int len = 0;
};