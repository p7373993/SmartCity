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
    // ������
    ADecalManager();

    // ��Į ���Ϳ��� ���� ���
    void RegisterBuildingInfluence(AActor* DecalActor, AActor* BuildingActor, float Influence);

    // ��� ������ ó���ϰ� �ǹ� ���� ������Ʈ
    UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
    void ApplyInfluences();

    // �Ŵ��� �ʱ�ȭ
    UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
    void ClearInfluences();

    FLinearColor GetBuildingColor(float Percentage);

    void AdjustBuildingColor(AActor* BuildingActor, float Percentage ,bool SetWhite);

    // �̱��� ���� �Լ�
    UFUNCTION(BlueprintCallable, Category = "DecalManager")
    static ADecalManager* GetInstance(UWorld* World);

protected:
    UPROPERTY(EditAnywhere, Category = Material, Meta = (AllowPrivateAccess = "true"))
    TObjectPtr<class UMaterialInterface> InstMaterial;
private:
    // �ǹ����� ��Į ������ ������ �����ϴ� ��
    TMap<AActor*, TArray<float>> BuildingInfluenceMap;

    // ������ �Լ�
    void DebugInfluences();

    // �̱��� �ν��Ͻ�
    static ADecalManager* Instance;
};
