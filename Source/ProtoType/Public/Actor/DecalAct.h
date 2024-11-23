// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DecalAct.generated.h"

UCLASS()
class PROTOTYPE_API ADecalAct : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADecalAct();
	void DetectBuildings();
	void AdjustBuildingHeight(AActor* BuildingActor);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	int Index;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
