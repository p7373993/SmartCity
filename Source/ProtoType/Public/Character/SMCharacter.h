// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SMCharacter.generated.h"

UCLASS()
class PROTOTYPE_API ASMCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASMCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


//기본 캐릭터 조작
protected:

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAcess = "true"))
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Move, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USMCharacterMoveComponent> DefaultMove;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Move, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USMPointComponent> RayCast;
};
