// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DecalAct.generated.h"

DECLARE_DELEGATE(FApplyInf);

UENUM(BlueprintType)
enum class ELandMarkType : uint8
{
	Bridge = 0,
	Stadium,
	CityMuseum,
	NationalMuseum,
	Hotel

};
UCLASS()
class PROTOTYPE_API ADecalAct : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADecalAct();
	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
	void DetectBuildings();//function2
	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
	void ClearVisual();
	void SetType(const ELandMarkType type);
	void AdjustBuildingHeight(AActor* BuildingActor, float PredictedPercent);
	void AdjustBuildingColor(AActor* BuildingActor, float Percentage);

	FLinearColor GetBuildingColor(float Percentage);

	bool IsHidden = false;
	int Index;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// JSON 데이터를 로드하는 함수 json load
	TMap<ELandMarkType, TMap<float, float>> LoadMachineLearningData(const FString& FilePath);

	// 선택된 랜드마크의 데이터를 가져오는 함수
	TMap<float, float> GetLandmarkData() const;

	UPROPERTY(EditAnywhere, Category = "Landmark")
	ELandMarkType SelectedLandMark;


	UPROPERTY(EditAnywhere, Category = Material, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UMaterialInterface> InstMaterial;


	// JSON 데이터 경로
	FString JsonPath = FPaths::ProjectContentDir() + TEXT("MachineLearningData.json");

	// 로드된 JSON 데이터를 저장하는 맵
	TMap<ELandMarkType, TMap<float, float>> MachineLearningData;

	TSet<AActor*> UniqueOverlappingActors; // 중복 방지를 위한 Set
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
