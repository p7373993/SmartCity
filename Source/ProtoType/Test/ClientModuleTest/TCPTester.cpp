#include "ProtoType/Test/ClientModuleTest/TCPTester.h"

ATCPTester::ATCPTester()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATCPTester::BeginPlay()
{
	Super::BeginPlay();
	TCPServer.TCPCunnect();
}

void ATCPTester::GetAPDataLOG()
{
	std::vector<APData> TempAPData;
	float TempAllData[20] = {0,0,200,200,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	TempAPData = TCPServer.GetAPData(TempAllData);
	for (const auto APDatas : TempAPData)
	{
		UE_LOG(LogTemp, Display, TEXT("%d : %d : %fl : %fl"), APDatas.ApartIndex, APDatas.floorInfo, APDatas.latitude, APDatas.longitude);
	}
}

void ATCPTester::GetSaleDataLOG()
{
	std::vector<SaleData> TempSaleData;
	float TempAllData[20] = { 0,0,200,200,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	TempSaleData = TCPServer.GetSaleData(TempAllData);
	for (const auto SaleDatas : TempSaleData)
	{
		UE_LOG(LogTemp, Display, TEXT("%d : %u : %d : %d"), SaleDatas.ApartIndex, SaleDatas.articleNo, SaleDatas.floorInfo, SaleDatas.dealOrWarrantPrc);
	}
}
