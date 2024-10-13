#include "ProtoType/private/ClientModule/TCPModule.h"

#pragma warning(disable:4996)

#include<stdio.h>
#include<iostream>
#include<Winsock2.h>
#include<vector>

#include <thread>
#include <chrono>

#include "ProtoType/Global/Properties.h"

#pragma comment(lib, "ws2_32.lib")

DEFINE_LOG_CATEGORY_STATIC(MyLogCategory, Warning, All);


std::vector<APData> TCPModule::GetAPData(float *Elemental,int ServerPort)
{
	SendingSelector(1, 8, Elemental, ServerPort);
	std::vector<APData> VAPDatas;

	int APSize;
	//--Recive001--Complite
	recv(Servers[ServerPort], (char*)&APSize, sizeof(APSize), 0);
	UE_LOG(LogTemp, Warning, TEXT("%d"), APSize);
	for (int32 i = 0; i < APSize; ++i)
	{
		APData TempAP;
		recv(Servers[ServerPort], (char*)&TempAP, sizeof(TempAP), 0);
		VAPDatas.push_back(TempAP);
	}
	IsInUse = false;
	return VAPDatas;
}

std::vector<SaleData> TCPModule::GetSaleData(float *Elemental, int ServerPort)
{
	SendingSelector(2, 8, Elemental, ServerPort);
	std::vector<SaleData> VSaleDatas;

	int SaleSize;
	//--Recive001--Complite
	recv(Servers[ServerPort], (char*)&SaleSize, sizeof(SaleSize), 0);
	UE_LOG(LogTemp, Warning, TEXT("%d"), SaleSize);
	for (int32 i = 0; i < SaleSize; ++i)
	{
		SaleData TempSaleData;
		recv(Servers[ServerPort], (char*)&TempSaleData, sizeof(TempSaleData), 0);
		VSaleDatas.push_back(TempSaleData);
	}
	IsInUse = false;
	return VSaleDatas;
}

TArray<SearchStruct> TCPModule::SearchBuildingData(const FString& SearchText, int ServerPort)
{
	float Elemental[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	SendingSelector(3, 0, Elemental, ServerPort);
	TArray<SearchStruct> SendingTextArray;
	int Size = 0;
	int index = 0;
	if (SearchText.Len() < 1024) {
		FString::Printf(TEXT("%s"), *SearchText).GetCharArray().GetData();
		strcpy_s(buffer, sizeof(buffer), TCHAR_TO_UTF8(*SearchText));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("SearchText is too long to fit in buffer!"));
	}

	send(Servers[ServerPort], buffer, sizeof(buffer), 0);
	recv(Servers[ServerPort], (char*)&Size, sizeof(Size), 0);
	for (int32 i = 0; i < Size; ++i)
	{
		SearchStruct TempStruct;

		recv(Servers[ServerPort], (char*)&buffer, sizeof(buffer), 0);
		TempStruct.BuildingName = UTF8_TO_TCHAR(buffer);
		recv(Servers[ServerPort], (char*)&buffer, sizeof(buffer), 0);
		TempStruct.tag = UTF8_TO_TCHAR(buffer);
		recv(Servers[ServerPort], (char*)&index, sizeof(index), 0);
		TempStruct.index = index;

		SendingTextArray.Add(TempStruct);
	}
	return SendingTextArray;
}

std::vector<SaleData> TCPModule::GetPRESaleData(float *Elemental, int ServerPort)
{
	return std::vector<SaleData>();
}

void TCPModule::CheckAndReconnect(int ServerPort)
{
	buffer[1024];
	int result = recv(Servers[ServerPort], buffer, sizeof(buffer), 0);

	if (result == SOCKET_ERROR) {
		if (WSAGetLastError() == WSAETIMEDOUT) {
			std::cerr << "recv timed out!" << std::endl;
		}
		else {
			std::cerr << "Connection lost. Attempting to reconnect..." << std::endl;
			closesocket(Servers[ServerPort]);

			TCPCunnect();
		}
	}
	else {
		std::cout << "Received data: " << buffer << std::endl;
	}
}

void TCPModule::SendingSelector(int Type, int MaxElIndex, float *Elemental, int ServerPort)
{
	IsInUse = true;
	SSelectorType Selector;
	Selector.Type = Type;
	Selector.MaxElIndex = MaxElIndex;
	std::copy(Elemental, Elemental + 20, Selector.Elemental);
	send(Servers[ServerPort], (char*)&Selector, sizeof(Selector), 0);
}

void TCPModule::TCPCunnect()
{	int ports[3] = { PORT1, PORT2, PORT3 };
	for (int i = 0; i < 3; i++) {
		WSADATA wsaData;
		SOCKADDR_IN addr;

		if (::WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
			HandleError("WSAStartup");
		Servers[i] = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (Servers[i] == INVALID_SOCKET)
			HandleError("socket");
		addr.sin_family = AF_INET;
		addr.sin_addr.S_un.S_addr = inet_addr(ServerIP);
		addr.sin_port = ::htons(ports[i]);

		int timeout = 2000;
		if (setsockopt(Servers[i], SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout)) == SOCKET_ERROR)
			HandleError("setsockopt(SO_RCVTIMEO)");
		if (setsockopt(Servers[i], SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout)) == SOCKET_ERROR)
			HandleError("setsockopt(SO_SNDTIMEO)");

		if (::connect(Servers[i], (const sockaddr*)&addr, sizeof(addr)) == INVALID_SOCKET)
			HandleError("connet");
	}
}

void TCPModule::HandleError(const char* cause)
{
	int errCode = ::WSAGetLastError();
}