// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MyPawn.generated.h"

UCLASS()
class PROTOTYPE_API AMyPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(Editanywhere, BlueprintReadOnly, Category = Input, Meta = (AllowProavateAccess = true));
	TObjectPtr<class UInputAction> QuaterMoveAction;

	void QuaterMove(const FInputActionValue& Value);
	//// Move the pawn forward/backward
	//void MoveForward(float Value);

	//// Move the pawn right/left
	//void MoveRight(float Value);

	//// Turn the pawn
	//void Turn(float Rate);

	//// Pitch the pawn
	//void LookUp(float Rate);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	    // Movement speed variable
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
    float MovementSpeed;

    // Turn rate variable
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
    float TurnRate;

    // Look up rate variable
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
    float LookUpRate;


};
