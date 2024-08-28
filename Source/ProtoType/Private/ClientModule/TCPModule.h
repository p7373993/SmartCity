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

//Input::Vertex of cube EX:) Elem[0]=latitude Elem[1]=longitude Elem[2]=latitude Elem[3]=longitude Elem[4]=latitude Elem[5]=longitude Elem[6]=latitude Elem[7]=longitude
//Output::Vector APData
    std::vector<APData> GetAPData(float Elemental[20]);

//Input::Vertex of cube EX:) Elem[0]=latitude Elem[1]=longitude Elem[2]=latitude Elem[3]=longitude Elem[4]=latitude Elem[5]=longitude Elem[6]=latitude Elem[7]=longitude
//Output::Vector SaleData
    std::vector<SaleData> GetSaleData(float Elemental[20]);

//Input::Vertex of cube EX:) Elem[0] InfraBuildingNameType
//Output::Vector float Distance, float Price Rate of increase
    std::vector<DisAndPRE> GetPREDecalData(float Elemental[20]);

//Input::Vertex of cube EX:) Elem[0]=latitude Elem[1]=longitude Elem[2]=latitude Elem[3]=longitude Elem[4]=latitude Elem[5]=longitude Elem[6]=latitude Elem[7]=longitude Elem[8]InfraBuildingNameType Elem[9]=latitude Elem[10]=longitude
//Output::Vector SaleData(PRE)
    std::vector<SaleData> GetPRESaleData(float Elemental[20]);

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