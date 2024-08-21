// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ViewLocationStruct.generated.h"

USTRUCT(BlueprintType)
struct FViewLocation
{
	GENERATED_BODY()
	
	FViewLocation(FVector2D InLeftUp, FVector2D InLeftDown, FVector2D InRightUp, FVector2D InRightDown) :
		LeftUp(InLeftUp),LeftDown(InLeftDown),RightUp(InRightUp),RightDown(InRightDown){}

	FViewLocation() //값이 없이 생성자 호출시 0으로 초기화
	{
		LeftUp = FVector2D(0.0f, 0.0f);
		LeftDown = FVector2D(0.0f, 0.0f);
		RightUp = FVector2D(0.0f, 0.0f);
		RightDown = FVector2D(0.0f, 0.0f);
	}

	UPROPERTY(BlueprintReadOnly)
	FVector2D LeftUp;

	UPROPERTY(BlueprintReadOnly)
	FVector2D LeftDown;

	UPROPERTY(BlueprintReadOnly)
	FVector2D RightUp;

	UPROPERTY(BlueprintReadOnly)
	FVector2D RightDown;
};
/**
 * 
 */

