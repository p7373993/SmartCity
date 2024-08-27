// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ClientModule/TCPModule.h"
#include "ProtoType/Global/Structs.h"
#include "SMPointComponent.generated.h"


UENUM(BlueprintType)
enum class EVisibleType : uint8
{
	None = 0,
	Price,
	Floor
};

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
	UFUNCTION(BlueprintCallable, Category = "View")//시작 함수
	FViewLocation GetCornerPoints();

	//레벨 바운드 좌표로 통시
	void LevelPoint();
	
	//좌표값 통신
	void GetPoint(FViewLocation& InLocation);
	void RayCast(const FVector& StartLocation, const FVector& EndLocation, const APData& Data);

	void latLongToXY(double latitude, double longitude, double& x, double& y);
	void XYTolatLong(double x, double y, double& latitude, double& longitude);

	//통신 모듈
	TCPModule& MyTCPModule = TCPModule::GetInstance();
	

	//beginplay이후 실행
	void OnceAfterBeginPlay();

	//머트리얼 교체 관련
protected:
	UPROPERTY(EditAnywhere, Category = Material, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UMaterialInterface> InstMaterial;

	void ChangeBuildingMaterial(FHitResult& HitResult, FLinearColor InNewColor);
	FLinearColor GetSpectrumColor(float Value);
//현재 타입정보
protected:
	UPROPERTY(EditAnywhere, Category = TypeControl, Meta = (AllowPrivateAccess = "true"))
	TMap<EVisibleType, class USMVisibleData*> TypeControlManager;

	EVisibleType CurrentType;

	void SetCurrentTypeData();
	//임시로 타입바꾸기
	UFUNCTION(BlueprintCallable)
	void TempChangeType(EVisibleType NewType = EVisibleType::None);

	float MaxValue;
	float MinValue;


		// 레벨로드 관련함수
protected:
	void OnLevelLoaded(ULevel* InLevel, UWorld* InWorld);

	void OnLevelLoadedWithOffset(ULevel* InLeve, UWorld* InWorld, const FVector& Offset, bool Inbool);
		


};
