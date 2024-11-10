// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ChartData.h"
#include "ProtoType/private/ClientModule/TCPModule.h"
#include <algorithm> // std::min_element, std::max_element
#include <vector>

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
	float A[20] = { 107678 ,10,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0};
	//std::vector<PriceData> Temp = MyTCPModule.GetSaleDataAccordingToDate(A, 2);
	std::vector<PriceData> Temp = MyTCPModule.GetPRESaleDataAccordingToDate(A, 2);

	for (size_t i = 0; i<Temp.size(); i++)
	{
		DataSet.Add(FVector2D(Temp[i].Date, Temp[i].Price));
	}
	//DataSet.Add(FVector2D(201806, 300));
	//DataSet.Add(FVector2D(201809, 500));
	//DataSet.Add(FVector2D(201812, 200));

	//UE_LOG(LogTemp, Display, TEXT("%f:%f"),Temp[1].Date,Temp[1].Price);


// Temp 벡터의 Price 값 중 최댓값과 최솟값 구하기
	MaxPrice = std::max_element(Temp.begin(), Temp.end(),
		[](const PriceData& a, const PriceData& b) {
			return a.Price < b.Price;
		})->Price;

	MinPrice = std::min_element(Temp.begin(), Temp.end(),
		[](const PriceData& a, const PriceData& b) {
			return a.Price < b.Price;
		})->Price;

	std::cout << "최댓값: " << MaxPrice << ", 최솟값: " << MinPrice << std::endl;

	
}

void UChartData::PredictData()//예측데이터 설정
{
	//std::vector<PriceData> Temp = MyTCPModule.predictData();// example

	//for (size_t i = 0; i < Temp.size(); i++)
	//{
	//	AIData.Add(FVector2D(Temp[i].Date, Temp[i].Price));
	//}
}

float UChartData::GetMinValue()
{
	return MinPrice;
}

float UChartData::GetMaxValue()
{
	return MaxPrice;
}


