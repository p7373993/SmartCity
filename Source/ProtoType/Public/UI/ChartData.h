// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChartData.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE_API UChartData : public UUserWidget
{
	GENERATED_BODY()
	
	


protected:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
	void SetData();

	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
	void PredictData();

	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
	float GetMinValue();

	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
	float GetMaxValue();


	UPROPERTY(BlueprintReadOnly, Category = "Data", meta = (AllowPrivateAccess = "true"))
	TArray<FVector2D> DataSet;

	UPROPERTY(BlueprintReadOnly, Category = "Data", meta = (AllowPrivateAccess = "true"))
	TArray<FVector2D> AIData;

	UPROPERTY(BlueprintReadOnly, Category = "Data", meta = (AllowPrivateAccess = "true"))
	float LowerBound;
	UPROPERTY(BlueprintReadOnly, Category = "Data", meta = (AllowPrivateAccess = "true"))
	float UpperBound;

	UPROPERTY(BlueprintReadOnly, Category = "Data", meta = (AllowPrivateAccess = "true"))
	float Num;

private:

	float MaxPrice;
	float MinPrice;
};
