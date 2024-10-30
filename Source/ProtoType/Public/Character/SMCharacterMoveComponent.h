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

	void GetActorTag();

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

	void StartMovingToLocation(const FVector& TargetLocation);
	void MoveToLocation();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void QuaterMove(const FInputActionValue& Value);
	void OnLeftClick(const FInputActionValue& Value);
	void OnRightClick(const FInputActionValue& Value);
	void UpDown(const FInputActionValue& Value);
	void Doubleclick(const FInputActionValue& Value);

	void OnDoubleClick();
	void OnSingleClick();

	bool bIsLeftClicking;
	bool bIsRightClicking;
	bool bIsMovingToTarget; //�̵� ����

	FVector TargetLocation;  // ��ǥ ��ġ
	FVector2D MouseLocation;
	APlayerController* PlayerController;

private:
	bool bIsClicking = false;        // ���� Ŭ�� ���¸� ����
	FTimerHandle ClickTimerHandle;   // Ÿ�̸� �ڵ�
	float DoubleClickMaxTime = 0.25f; // ���� Ŭ������ ������ �ִ� �ð� (0.25��)
};
