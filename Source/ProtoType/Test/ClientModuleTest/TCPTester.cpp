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
	float TempAllData[20] = {0,0,200,200,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	TempAPData = TCPServer.GetAPData(TempAllData);
	for (const auto APDatas : TempAPData)
	{
		FString ABC;
		for (int Aindex=0; Aindex < 20; Aindex = Aindex+1)
		{
			//ABC.Append(FString::Printf(TEXT("%c%c"), APDatas.buildingName[Aindex]));

			


		}
		int len = MultiByteToWideChar(CP_ACP, 0, APDatas.buildingName, -1, NULL, 0);
		std::wstring wstr(len, 0);
		MultiByteToWideChar(CP_ACP, 0, APDatas.buildingName, -1, &wstr[0], len);

		int utf8_len = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
		std::string utf8_str(utf8_len, 0);
		WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &utf8_str[0], utf8_len, NULL, NULL);
		FString FStringFromUTF8 = TEXT("한글");

		UE_LOG(LogTemp, Display, TEXT("%s"), *FStringFromUTF8);
	}
}