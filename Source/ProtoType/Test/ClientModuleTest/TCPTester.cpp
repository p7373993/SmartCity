#include "ProtoType/Test/ClientModuleTest/TCPTester.h"
#include "Async/Async.h"

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
	if (TCPServer.IsInUse == false)
	{
		Async(EAsyncExecution::Thread, [this]() {ThreadTest(); });
	}
}

void ATCPTester::ThreadTest()
{
	std::vector<APData> TempAPData;
	float TempAllData[20] = { 0, 0, 200, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	TempAPData = TCPServer.GetAPData(TempAllData);
	Async(EAsyncExecution::TaskGraphMainThread, [TempAPData]() {
		for (const auto& APDatas : TempAPData)
		{
			UE_LOG(LogTemp, Display, TEXT("%d : %d : %f : %f"), APDatas.ApartIndex, APDatas.floorInfo, APDatas.latitude, APDatas.longitude);
		}
		});
}

void ATCPTester::GetSaleDataLOG()
{
	std::vector<SaleData> TempSaleData;
	float TempAllData[20] = { 0,0,200,200,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	TempSaleData = TCPServer.GetSaleData(TempAllData);

}
