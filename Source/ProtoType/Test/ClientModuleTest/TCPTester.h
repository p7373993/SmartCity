// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ProtoType/private/ClientModule/TCPModule.h"
#include "ProtoType/Global/Structs.h"
#include "vector"
#include "TCPTester.generated.h"

UCLASS()
class PROTOTYPE_API ATCPTester : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATCPTester();

	UFUNCTION(BlueprintCallable)
	void GetAPDataLOG();

	UFUNCTION(BlueprintCallable)
	void GetSaleDataLOG();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	TCPModule& TCPServer = TCPModule::GetInstance();
};
