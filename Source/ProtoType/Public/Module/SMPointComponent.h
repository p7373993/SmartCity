// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SMPointComponent.generated.h"

USTRUCT(BlueprintType)
struct FViewLocation
{
	GENERATED_BODY()

	FViewLocation(FVector2D InLeftUp, FVector2D InLeftDown, FVector2D InRightUp, FVector2D InRightDown) :
		LeftUp(InLeftUp), LeftDown(InLeftDown), RightUp(InRightUp), RightDown(InRightDown) {}

	FViewLocation() //값이 없이 생성자 호출시 0으로 초기화
	{
		LeftUp = FVector2D(0.0f, 0.0f);
		LeftDown = FVector2D(0.0f, 0.0f);
		RightUp = FVector2D(0.0f, 0.0f);
		RightDown = FVector2D(0.0f, 0.0f);
	}

	TArray<FVector2D> GetArray() const
	{
		return { LeftUp, LeftDown, RightUp, RightDown };
	}

	UPROPERTY(BlueprintReadOnly)
	FVector2D LeftUp;

	UPROPERTY(BlueprintReadOnly)
	FVector2D LeftDown;

	UPROPERTY(BlueprintReadOnly)
	FVector2D RightUp;

	UPROPERTY(BlueprintReadOnly)
	FVector2D RightDown;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROTOTYPE_API USMPointComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USMPointComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	//주인 캐릭터
	UPROPERTY(VisibleAnywhere, Category = "Actor")
	TObjectPtr<ACharacter> OwningActor;

	//코너 구하기
	UFUNCTION(BlueprintCallable, Category = "View")
	FViewLocation GetCornerPoints();
	
	//좌표값 통신
	void GetPoint(FViewLocation& InLocation);

	void latLongToXY(double latitude, double longitude, double& x, double& y);
	void XYTolatLong(double x, double y, double& latitude, double& longitude);

};
