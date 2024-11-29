// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/DecalManager.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "Components/StaticMeshComponent.h"
#include "EngineUtils.h"


ADecalManager* ADecalManager::Instance = nullptr;


// Sets default values
ADecalManager::ADecalManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


    static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialFinder(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Sungwoo/M_DynamicInst.M_DynamicInst'"));

    if (MaterialFinder.Succeeded())
    {
        InstMaterial = MaterialFinder.Object;
    }
    // �̱��� �ν��Ͻ� ���
    if (!Instance)
    {
        Instance = this;
    }
}

void ADecalManager::RegisterBuildingInfluence(AActor* DecalActor, AActor* BuildingActor, float Influence)
{
    if (!BuildingInfluenceMap.Contains(BuildingActor))
    {
        // �ǹ��� ���� �ʱ� �迭 ����
        BuildingInfluenceMap.Add(BuildingActor, TArray<float>());
    }

    // ���⵵�� ����
    BuildingInfluenceMap[BuildingActor].Add(Influence);

    // ����� �α�
    UE_LOG(LogTemp, Log, TEXT("Decal Actor %s influenced Building %s with %f%%"),
        *DecalActor->GetName(), *BuildingActor->GetName(), Influence);
}

void ADecalManager::ApplyInfluences()
{
    // �� �ǹ��� ���� ������ ���� ����
    for (const auto& Pair : BuildingInfluenceMap)
    {
        AActor* BuildingActor = Pair.Key;
        const TArray<float>& Influences = Pair.Value;

        // ������ ������ ��� (�������� ���� ����)
        float FinalInfluence = 1.0f;
        for (float Influence : Influences)
        {
            FinalInfluence *= (1.0f + Influence / 100.0f);
        }

        // ���� ������ 1.0�� ���� 100% �������� ���
        FinalInfluence = (FinalInfluence - 1.0f) * 100.0f;

        AdjustBuildingColor(BuildingActor, FinalInfluence,false);

        // ����� �α�
        UE_LOG(LogTemp, Log, TEXT("Building %s has final influence %f%%"), *BuildingActor->GetName(), FinalInfluence);
    }

    // ������ ���� ���
    DebugInfluences();

}

void ADecalManager::ClearInfluences()
{

    TArray<AActor*> Keys;
    BuildingInfluenceMap.GetKeys(Keys);

    
    FLinearColor WhiteColor = FLinearColor::White;
    for (const AActor* actor : Keys)
    {
    
        UStaticMeshComponent* Mesh = actor->FindComponentByClass<UStaticMeshComponent>();
        if (Mesh)
        {
            // ���� ��Ƽ���� �ν��Ͻ� ����
            if (InstMaterial)
            {
                UMaterialInstanceDynamic* DynamicMaterialInstance = UMaterialInstanceDynamic::Create(InstMaterial, this);
                if (DynamicMaterialInstance)
                {
                    // ��Ƽ������ Color �Ķ���� ����
                    DynamicMaterialInstance->SetVectorParameterValue(FName("Color"), WhiteColor);

                    // ����ƽ �޽� ������Ʈ�� ��Ƽ���� ����
                    Mesh->SetMaterial(0, DynamicMaterialInstance);
                }
            }
        }
    }

    BuildingInfluenceMap.Empty();
}

FLinearColor ADecalManager::GetBuildingColor(float Percentage)
{
    // �ۼ�Ʈ ���� ä���� ���� (0~50 -> 0.5~1.0)
    float Saturation = FMath::GetMappedRangeValueClamped(
        FVector2D(0.0f, 40.0f), FVector2D(50.0f, 255.0f), FMath::Abs(Percentage)
    );

    // ��� ���� (������ �Ǵ� �������� ���� ����)
    float Value = 1.0f; // ��⸦ ����

    if (Percentage >= 0)
    {
        float Hue = 160.0f; // �Ķ��� (Hue: 240)
        //return FLinearColor::MakeFromHSV8(Hue, Saturation * 255, Value * 255);
        return FLinearColor::MakeFromHSV8(160, Saturation, 240);
    }
    else
    {
        float Hue = 0.0f; // ������ (Hue: 0)
        return FLinearColor::MakeFromHSV8(Hue, Saturation, 240);
    }
}

void ADecalManager::AdjustBuildingColor(AActor* BuildingActor, float Percentage, bool SetWhite)
{
    // ���� ���
    FLinearColor NewColor = GetBuildingColor(Percentage);
    UE_LOG(LogTemp, Log, TEXT("Generated Color: R=%f, G=%f, B=%f, A=%f"), NewColor.R, NewColor.G, NewColor.B, NewColor.A);
    // Static Mesh ������Ʈ ã��
    UStaticMeshComponent* Mesh = BuildingActor->FindComponentByClass<UStaticMeshComponent>();
    if (Mesh)
    {
        // ���� ��Ƽ���� �ν��Ͻ� ����
        if (InstMaterial)
        {
            UMaterialInstanceDynamic* DynamicMaterialInstance = UMaterialInstanceDynamic::Create(InstMaterial, this);
            if (DynamicMaterialInstance)
            {
                // ��Ƽ������ Color �Ķ���� ����
                DynamicMaterialInstance->SetVectorParameterValue(FName("Color"), NewColor);

                // ����ƽ �޽� ������Ʈ�� ��Ƽ���� ����
                Mesh->SetMaterial(0, DynamicMaterialInstance);
            }
        }
    }
}

ADecalManager* ADecalManager::GetInstance(UWorld* World)
{
    if (!Instance)
    {
        // ���忡�� �Ŵ��� �˻�
        for (TActorIterator<ADecalManager> It(World); It; ++It)
        {
            Instance = *It;
            break;
        }

        // �Ŵ����� ������ ���� ����
        if (!Instance)
        {
            Instance = World->SpawnActor<ADecalManager>();
        }
    }
    return Instance;
}

void ADecalManager::DebugInfluences()
{
    for (const auto& Pair : BuildingInfluenceMap)
    {
        AActor* BuildingActor = Pair.Key;
        const TArray<float>& Influences = Pair.Value;

        FString InfluenceLog;
        for (float Influence : Influences)
        {
            InfluenceLog += FString::Printf(TEXT("%f, "), Influence);
        }

        UE_LOG(LogTemp, Log, TEXT("Building %s has influences: %s"),
            *BuildingActor->GetName(), *InfluenceLog);
    }
}


