// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/WidgetComponent.h"
#include "Manager/DecalActSpawnManager.h"
#include "Blueprint/UserWidget.h"
#include "UIActor.generated.h"

UCLASS()
class PROTOTYPE_API AUIActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUIActor();
	DecalActSpawnManager& Manager = DecalActSpawnManager::GetInstance();
	void SpawnAct();

	void DeleteAct();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vector")
	float latitude;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vector")
	float longitude;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vector")
	int index;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	TObjectPtr<class UWidgetComponent> WidgetComponent;

};
