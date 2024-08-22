// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputActionValue.h"
#include "SMCharacterMoveComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROTOTYPE_API USMCharacterMoveComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USMCharacterMoveComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	UPROPERTY(VisibleAnywhere,Category = "Actor")
	TObjectPtr<ACharacter> OwningActor;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputMappingContext* InputMapping;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* LeftClickAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* RightClickAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* UpDownAction;

	void QuaterMove(const FInputActionValue& Value);
	void OnLeftClick(const FInputActionValue& Value);
	void OnRightClick(const FInputActionValue& Value);
	void UpDown(const FInputActionValue& Value);

	bool bIsLeftClicking;
	bool bIsRightClicking;

		
};
