// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OpenWorldComponent.generated.h"

//레벨 로드 델리게이트
DECLARE_DELEGATE(FOnLevelLoaded);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROTOTYPE_API UOpenWorldComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenWorldComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 레벨로드 관련함수
protected:
	void OnLevelLoaded(ULevel* InLevel, UWorld* InWorld);
		
	FOnLevelLoaded LevelLoad;
};
