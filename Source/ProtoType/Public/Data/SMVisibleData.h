// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SMVisibleData.generated.h"

/**
 * 
 */

UCLASS()
class PROTOTYPE_API USMVisibleData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, Category = "Value")
	float MaxValue;

	UPROPERTY(EditAnywhere, Category = "Value")
	float MinValue;
};
