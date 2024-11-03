// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ChartData.h"
#include "ProtoType/private/ClientModule/TCPModule.h"

//#include <algorithm> // std::min_element, std::max_element
//#include <vector>

void UChartData::NativeConstruct()
{
	Super::NativeConstruct();

	Num = 50;
}

void UChartData::SetData()
{
	TCPModule& MyTCPModule = TCPModule::GetInstance();
	float A[20] = { 107678 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0};
	std::vector<PriceData> Temp = MyTCPModule.GetSaleDataAccordingToDate(A, 2);

	for (size_t i = 0; i<30; i++)
	{
		DataSet.Add(FVector2D(Temp[i].Date, Temp[i].Price));
	}
	//DataSet.Add(FVector2D(201806, 300));
	//DataSet.Add(FVector2D(201809, 500));
	//DataSet.Add(FVector2D(201812, 200));

	//UE_LOG(LogTemp, Display, TEXT("%f:%f"),Temp[1].Date,Temp[1].Price);


	
}


