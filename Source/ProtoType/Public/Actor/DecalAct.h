// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DecalAct.generated.h"

UENUM(BlueprintType)
enum class ELandMarkType : uint8
{
	Bridge = 0,
	Stadium,
	CityMuseum,
	NationalMuseum,
	Hotel

};
UCLASS()
class PROTOTYPE_API ADecalAct : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADecalAct();
	void DetectBuildings();
	void AdjustBuildingHeight(AActor* BuildingActor, float PredictedPercent);
	void AdjustBuildingColor(AActor* BuildingActor, float Percentage);

	FLinearColor GetBuildingColor(float Percentage);

	bool IsHidden = false;
	int Index;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UPROPERTY(EditAnywhere, Category = Material, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UMaterialInterface> InstMaterial;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
