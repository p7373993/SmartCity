#pragma once

#include "CoreMinimal.h"
#include "ProtoType/Global/Structs.h"
#include <vector>
#include <stdio.h>
#include <iostream>
#include <Winsock2.h>
#include <string.h>


class PROTOTYPE_API TCPModule
{
public:

//DeclarationTCPModule //--TCPModule& MyTCPModule = TCPModule::GetInstance();--//
    static TCPModule& GetInstance()
    {
        static TCPModule instance;
        return instance;
    }

//Run only once in Game But if Run this when the connection is lost//
    void TCPCunnect();

//Input::Vertex of cube EX:) Elem[0]=(0,0); Elem[1]=(0,1); Elem[2]=(1,1); Elem[3]=(1,0); 
//Output::Vector APData
    std::vector<APData> GetAPData(std::vector<float> Elemental);

//Input::Vector APIndex Ex:) {0,1,2,3,4,5......99}
//Output::Vector SaleData
    std::vector<SaleData> GetSaleData(std::vector<int> Elemental);

    TCPModule(const TCPModule&) = delete;
    TCPModule& operator=(const TCPModule&) = delete;


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