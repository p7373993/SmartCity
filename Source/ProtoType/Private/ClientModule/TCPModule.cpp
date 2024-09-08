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


std::vector<APData> TCPModule::GetAPData(float Elemental[20])
{
	SendingSelector(1, 8, Elemental);
	std::vector<APData> VAPDatas;

	int APSize;
	//--Recive001--Complite
	recv(Server, (char*)&APSize, sizeof(APSize), 0);
	UE_LOG(LogTemp, Warning, TEXT("%d"), APSize);
	for (int32 i = 0; i < APSize; ++i)
	{
		APData TempAP;
		recv(Server, (char*)&TempAP, sizeof(TempAP), 0);
		VAPDatas.push_back(TempAP);
	}
	IsInUse = false;
	return VAPDatas;
}

std::vector<SaleData> TCPModule::GetSaleData(float Elemental[20])
{
	SendingSelector(2, 8, Elemental);
	std::vector<SaleData> VSaleDatas;

	int SaleSize;
	//--Recive001--Complite
	recv(Server, (char*)&SaleSize, sizeof(SaleSize), 0);
	UE_LOG(LogTemp, Warning, TEXT("%d"), SaleSize);
	for (int32 i = 0; i < SaleSize; ++i)
	{
		SaleData TempSaleData;
		recv(Server, (char*)&TempSaleData, sizeof(TempSaleData), 0);
		VSaleDatas.push_back(TempSaleData);
	}
	IsInUse = false;
	return VSaleDatas;
}

std::vector<DisAndPRE> TCPModule::GetPREDecalData(float Elemental[20])
{
	return std::vector<DisAndPRE>();
}

std::vector<SaleData> TCPModule::GetPRESaleData(float Elemental[20])
{
	return std::vector<SaleData>();
}

void TCPModule::CheckAndReconnect()
{
	buffer[1024];
	int result = recv(Server, buffer, sizeof(buffer), 0);

	if (result == SOCKET_ERROR) {
		if (WSAGetLastError() == WSAETIMEDOUT) {
			std::cerr << "recv timed out!" << std::endl;
		}
		else {
			std::cerr << "Connection lost. Attempting to reconnect..." << std::endl;
			closesocket(Server);

			TCPCunnect();
		}
	}
	else {
		std::cout << "Received data: " << buffer << std::endl;
	}
}

void TCPModule::SendingSelector(int Type, int MaxElIndex, float Elemental[20])
{
	IsInUse = true;
	SSelectorType Selector;
	Selector.Type = Type;
	Selector.MaxElIndex = MaxElIndex;
	std::copy(Elemental, Elemental + 20, Selector.Elemental);
	send(Server, (char*)&Selector, sizeof(Selector), 0);
}

void TCPModule::TCPCunnect()
{
	if (::WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
		HandleError("WSAStartup");
	Server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (Server == INVALID_SOCKET)
		HandleError("socket");
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = inet_addr(ServerIP);
	addr.sin_port = ::htons(PORT);

	int timeout = 2000;
	if (setsockopt(Server, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout)) == SOCKET_ERROR)
		HandleError("setsockopt(SO_RCVTIMEO)");
	if (setsockopt(Server, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout)) == SOCKET_ERROR)
		HandleError("setsockopt(SO_SNDTIMEO)");

	if (::connect(Server, (const sockaddr*)&addr, sizeof(addr)) == INVALID_SOCKET)
		HandleError("connet");
	return;
}

void TCPModule::HandleError(const char* cause)
{
	int errCode = ::WSAGetLastError();
}