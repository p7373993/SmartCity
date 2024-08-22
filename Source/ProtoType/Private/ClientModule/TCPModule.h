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
    std::vector<APData> GetAPData(float Elemental[20]);

    //Input::Vertex of cube EX:) Elem[0]=(0,0); Elem[1]=(0,1); Elem[2]=(1,1); Elem[3]=(1,0);
//Output::Vector SaleData
    std::vector<SaleData> GetSaleData(float Elemental[20]);

    TCPModule(const TCPModule&) = delete;
    TCPModule& operator=(const TCPModule&) = delete;


private:
    TCPModule() {};

    void SendingSelector(int Type, int MaxElIndex, float Elemental[20]);

    void HandleError(const char* cause);

    SOCKET Server;
    WSADATA wsaData;
    SOCKADDR_IN addr;
    char buffer[1024];
    int len = 0;
};