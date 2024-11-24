// Fill out your copyright notice in the Description page of Project Settings.

#include "Manager/TCPDataManager.h"

TCPDataManager::TCPDataManager()
{
	MyTCPModule.TCPCunnect();
	float TemplA[20] = {0,0,360,360,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	TempAPData = MyTCPModule.GetAPData(TemplA, 0);
	TempSaleData = MyTCPModule.GetSaleData(TemplA, 0);
}