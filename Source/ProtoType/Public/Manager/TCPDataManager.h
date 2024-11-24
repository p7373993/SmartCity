// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "vector"
#include "ProtoType/Global/Structs.h"
#include "ClientModule/TCPModule.h"
/**
 * 
 */
class PROTOTYPE_API TCPDataManager
{
public:
    static TCPDataManager& GetInstance()
    {
        static TCPDataManager instance;
        return instance;
    }
	TCPDataManager();
    TCPModule& MyTCPModule = TCPModule::GetInstance();

    bool IsInUse = false;	
    std::vector<APData> TempAPData;
    std::vector<SaleData> TempSaleData;
};