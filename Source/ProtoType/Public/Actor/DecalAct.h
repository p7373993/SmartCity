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

	// JSON �����͸� �ε��ϴ� �Լ� json load
	TMap<ELandMarkType, TMap<float, float>> LoadMachineLearningData(const FString& FilePath);

	// ���õ� ���帶ũ�� �����͸� �������� �Լ�
	TMap<float, float> GetLandmarkData() const;

	UPROPERTY(EditAnywhere, Category = "Landmark")
	ELandMarkType SelectedLandMark;


	UPROPERTY(EditAnywhere, Category = Material, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UMaterialInterface> InstMaterial;


	// JSON ������ ���
	FString JsonPath = FPaths::ProjectContentDir() + TEXT("MachineLearningData.json");

	// �ε�� JSON �����͸� �����ϴ� ��
	TMap<ELandMarkType, TMap<float, float>> MachineLearningData;

	TSet<AActor*> UniqueOverlappingActors; // �ߺ� ������ ���� Set
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
