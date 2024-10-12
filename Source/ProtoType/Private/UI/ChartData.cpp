// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ChartData.h"

void UChartData::NativeConstruct()
{
	Super::NativeConstruct();


}

void UChartData::SetData()
{

	DataSet.Add(FVector2D(202300, 100));
	DataSet.Add(FVector2D(202303, 200));
	DataSet.Add(FVector2D(202306, 150));
	DataSet.Add(FVector2D(202309, 100));
	DataSet.Add(FVector2D(202312, 70));

}
