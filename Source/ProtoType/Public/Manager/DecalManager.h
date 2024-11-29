// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DecalManager.generated.h"

UCLASS()
class PROTOTYPE_API ADecalManager : public AActor
{
	GENERATED_BODY()
	

public:
    // 생성자
    ADecalManager();

    // 데칼 액터에서 영향 등록
    void RegisterBuildingInfluence(AActor* DecalActor, AActor* BuildingActor, float Influence);

    // 모든 영향을 처리하고 건물 상태 업데이트
    UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
    void ApplyInfluences();

    // 매니저 초기화
    UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
    void ClearInfluences();

    FLinearColor GetBuildingColor(float Percentage);

    void AdjustBuildingColor(AActor* BuildingActor, float Percentage ,bool SetWhite);

    // 싱글톤 접근 함수
    UFUNCTION(BlueprintCallable, Category = "DecalManager")
    static ADecalManager* GetInstance(UWorld* World);

protected:
    UPROPERTY(EditAnywhere, Category = Material, Meta = (AllowPrivateAccess = "true"))
    TObjectPtr<class UMaterialInterface> InstMaterial;
private:
    // 건물별로 데칼 액터의 영향을 저장하는 맵
    TMap<AActor*, TArray<float>> BuildingInfluenceMap;

    // 디버깅용 함수
    void DebugInfluences();

    // 싱글톤 인스턴스
    static ADecalManager* Instance;
};
