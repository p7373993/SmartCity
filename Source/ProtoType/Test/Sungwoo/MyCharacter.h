// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "ViewLocationStruct.h"
#include "ProtoType/ClientModule/TCPModule.h"
#include "MyCharacter.generated.h"


UCLASS()
class PROTOTYPE_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere,meta = (AllowPrivateAcess = "true"))
	class UCameraComponent* Camera;

protected:

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

	UFUNCTION(BlueprintCallable, Category = "View")
	FViewLocation GetCornerPoints();

	UFUNCTION(BlueprintCallable, Category = "View")
	void TempRayCast();

	virtual void RayCast(const FVector& StartLocation, const FVector& EndLocation, int32 FloorInfo = 10);
	virtual void GetPoint(FVector2D LU, FVector2D LD, FVector2D RU, FVector2D RD);
public:
	// Sets default values for this character's properties
	AMyCharacter();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



	TCPModule& MyTCPModule = TCPModule::GetInstance();
private:
	bool bIsLeftClicking;
	bool bIsRightClicking;






};
