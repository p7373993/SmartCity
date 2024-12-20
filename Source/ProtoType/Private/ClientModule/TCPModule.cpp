#include "ProtoType/private/ClientModule/TCPModule.h"

#pragma warning(disable:4996)

#include<stdio.h>
#include<iostream>
#include<Winsock2.h>
#include<vector>
#include<map>
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
		UE_LOG(LogTemp, Warning, TEXT("%s"), *TempStruct.BuildingName);
		recv(Servers[ServerPort], (char*)&buffer, sizeof(buffer), 0);
		TempStruct.tag = UTF8_TO_TCHAR(buffer);
		UE_LOG(LogTemp, Warning, TEXT("%s"), *TempStruct.tag);
		recv(Servers[ServerPort], (char*)&index, sizeof(index), 0);
		TempStruct.index = index;
		UE_LOG(LogTemp, Warning, TEXT("%d"), TempStruct.index);

		SendingTextArray.Add(TempStruct);
	}
	return SendingTextArray;
}

std::vector<float> TCPModule::GetDecalDistance(float* Elemental, int ServerPort)
{
	SendingSelector(8, 8, Elemental, ServerPort);
	std::vector<float> Data;

	int DataSize;
	recv(Servers[ServerPort], (char*)&DataSize, sizeof(DataSize), 0);
	for (int32 i = 0; i < DataSize; ++i)
	{
		float TempPersent;
		recv(Servers[ServerPort], (char*)&TempPersent, sizeof(TempPersent), 0);
		Data.push_back(TempPersent);
	}
	IsInUse = false;
	return Data;
}

std::vector<SaleData> TCPModule::GetPRESaleData(float *Elemental, int ServerPort)
{
	return std::vector<SaleData>();
}

std::vector<PriceData> TCPModule::GetSaleDataAccordingToDate(float* Elemental, int ServerPort)
{
	SendingSelector(4, 1, Elemental, ServerPort);
	std::vector<PriceData> Data;
	int Size;
	recv(Servers[ServerPort], (char*)&Size, sizeof(Size), 0);
	for (int i = 0; i < Size; ++i)
	{
		PriceData DataTemp;
		recv(Servers[ServerPort], (char*)&DataTemp, sizeof(DataTemp), 0);
		Data.push_back(DataTemp);
	}
	return Data;
}
std::vector<PriceData> TCPModule::GetPRESaleDataAccordingToDate(float* Elemental, int ServerPort)
{
	SendingSelector(6, 1, Elemental, ServerPort);
	std::vector<PriceData> Data;
	int Size;
	fd_set read_fds;
	FD_ZERO(&read_fds);
	FD_SET(Servers[ServerPort], &read_fds);
	struct timeval timeout;
	timeout.tv_sec = 30;
	timeout.tv_usec = 0;
	int select_result = select(Servers[ServerPort] + 1, &read_fds, NULL, NULL, &timeout);
	if (select_result > 0) {
		if (recv(Servers[ServerPort], (char*)&Size, sizeof(Size), 0) <= 0) {
			std::cerr << "Error: Failed to receive data size\n";
			return Data;
		}
		for (int i = 0; i < Size; ++i) {
			PriceData DataTemp;
			if (recv(Servers[ServerPort], (char*)&DataTemp, sizeof(DataTemp), 0) <= 0) {
				std::cerr << "Error: Failed to receive price data\n";
				break;
			}
			Data.push_back(DataTemp);
		}
	}
	else if (select_result == 0) {
		std::cerr << "Timeout: Server did not respond within 30 seconds.\n";
	}
	else {
		std::cerr << "Error: select failed\n";
	}
	return Data;
}
TextStruct TCPModule::GetBuildingAddressAndName(float* Elemental, int ServerPort)
{
	SendingSelector(5, 0, Elemental, ServerPort);
	TArray<SearchStruct> SendingTextArray;
	TextStruct TempStruct;
	recv(Servers[ServerPort], (char*)&buffer, sizeof(buffer), 0);
	TempStruct.BuildingName = UTF8_TO_TCHAR(buffer);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *TempStruct.BuildingName);
	recv(Servers[ServerPort], (char*)&buffer, sizeof(buffer), 0);
	TempStruct.BuildingAddress = UTF8_TO_TCHAR(buffer);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *TempStruct.BuildingAddress);
	return TempStruct;
}

std::vector<float> TCPModule::GetXYLocation(float* Elemental, int ServerPort)
{
	SendingSelector(7, 0, Elemental, ServerPort);
	float TempX = 0;
	float TempY = 0;
	recv(Servers[ServerPort], (char*)&TempX, sizeof(TempX), 0);
	recv(Servers[ServerPort], (char*)&TempY, sizeof(TempY), 0);
	std::vector<float> Data;
	Data.push_back(TempX);
	Data.push_back(TempY);
	return Data;
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
{	
	int ports[3] = { PORT1, PORT2, PORT3 };
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

void TCPModule::TCPReCunnect()
{
	closesocket(Servers[0]);

	closesocket(Servers[1]);

	closesocket(Servers[2]);
	TCPCunnect();
}

void TCPModule::HandleError(const char* cause)
{
	int errCode = ::WSAGetLastError();
}