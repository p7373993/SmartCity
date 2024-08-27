// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BuildingInfoComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROTOTYPE_API UBuildingInfoComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBuildingInfoComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//건물 정보
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Price;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 FloorNumber;
		
};
