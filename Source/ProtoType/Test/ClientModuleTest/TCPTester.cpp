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
	TempAPData = TCPServer.GetAPData(TempAllData, 0);
	Async(EAsyncExecution::TaskGraphMainThread, [&,TempAPData]() {
		for (const auto& APDatas : TempAPData)
		{
			Test(APDatas);
		}
		});
}

void ATCPTester::Test(APData ad)
{
	UE_LOG(LogTemp, Display, TEXT("%d : %d : %f : %f"), ad.ApartIndex, ad.floorInfo, ad.latitude, ad.longitude);
}

void ATCPTester::GetSaleDataLOG()
{
	std::vector<SaleData> TempSaleData;
	float TempAllData[20] = { 0,0,200,200,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	TempSaleData = TCPServer.GetSaleData(TempAllData, 0);

}
