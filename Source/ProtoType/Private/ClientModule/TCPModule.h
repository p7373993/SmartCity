#pragma once

#include "CoreMinimal.h"
#include "ProtoType/Global/Structs.h"
#include <vector>
#include <stdio.h>
#include <iostream>
#include <Winsock2.h>
#include <string.h>

//---Use EX:)----

//#include "ProtoType/Test/ClientModuleTest/TCPTester.h"
//#include "Async/Async.h"

/*
    if (TCPServer.IsInUse == false)
    {
        Async(EAsyncExecution::Thread, [this]() {ThreadTest(); });
    }
*/

//GetData Function
/*void User::GetData()
{
    std::vector<APData> Data;
    float Element[20] = {Elem};
    Data = TCPServer.GetAPData(Element);
    Async(EAsyncExecution::TaskGraphMainThread, [Data]() {
        for (const auto& APDatas : Data )
        {
            //Data Post-processing
        }
        });
}*/

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
    std::vector<APData> GetAPData(float *Elemental, int ServerPort);

//Input::Vertex of cube EX:) Elem[0]=latitude Elem[1]=longitude Elem[2]=latitude Elem[3]=longitude Elem[4]=latitude Elem[5]=longitude Elem[6]=latitude Elem[7]=longitude
//Output::Vector SaleData
    std::vector<SaleData> GetSaleData(float *Elemental, int ServerPort);

//Input::Vertex of cube EX:) Elem[0] InfraBuildingNameType
//Output::Vector float Distance, float Price Rate of increase
//  std::vector<DisAndPRE> GetPREDecalData(float *Elemental);

    TArray<SearchStruct> SearchBuildingData(const FString& SearchText, int ServerPort);

//Input::Index and Persent EX:) Elem[0] MaxIndex Elem[1~19]=Persent
//Output::DecalActDistance in nearest order
    std::vector<float> GetDecalDistance(float* Elemental, int ServerPort);

//Input::Vertex of cube EX:) Elem[0]=latitude Elem[1]=longitude Elem[2]=latitude Elem[3]=longitude Elem[4]=latitude Elem[5]=longitude Elem[6]=latitude Elem[7]=longitude Elem[8]InfraBuildingNameType Elem[9]=latitude Elem[10]=longitude
//Output::Vector SaleData(PRE)
    std::vector<SaleData> GetPRESaleData(float *Elemental, int ServerPort);

    TextStruct GetBuildingAddressAndName(float* Elemental, int ServerPort);

    TCPModule(const TCPModule&) = delete;
    TCPModule& operator=(const TCPModule&) = delete;

    bool IsInUse = false;

private:
    TCPModule() {};

    void CheckAndReconnect(int ServerPort);

    void SendingSelector(int Type, int MaxElIndex, float *Elemental, int ServerPort);

    void HandleError(const char* cause);

    SOCKET Servers[3];
    char buffer[1024];
    int len = 0;
};