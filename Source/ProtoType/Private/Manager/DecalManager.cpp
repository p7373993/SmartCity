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
    // 싱글톤 인스턴스 등록
    if (!Instance)
    {
        Instance = this;
    }
}

void ADecalManager::RegisterBuildingInfluence(AActor* DecalActor, AActor* BuildingActor, float Influence)
{
    if (!BuildingInfluenceMap.Contains(BuildingActor))
    {
        // 건물에 대해 초기 배열 생성
        BuildingInfluenceMap.Add(BuildingActor, TArray<float>());
    }

    // 영향도를 누적
    BuildingInfluenceMap[BuildingActor].Add(Influence);

    // 디버깅 로그
    UE_LOG(LogTemp, Log, TEXT("Decal Actor %s influenced Building %s with %f%%"),
        *DecalActor->GetName(), *BuildingActor->GetName(), Influence);
}

void ADecalManager::ApplyInfluences()
{
    // 각 건물에 대해 누적된 영향 적용
    for (const auto& Pair : BuildingInfluenceMap)
    {
        AActor* BuildingActor = Pair.Key;
        const TArray<float>& Influences = Pair.Value;

        // 누적된 영향을 계산 (곱셈으로 영향 누적)
        float FinalInfluence = 1.0f;
        for (float Influence : Influences)
        {
            FinalInfluence *= (1.0f + Influence / 100.0f);
        }

        // 최종 값에서 1.0을 빼서 100% 기준으로 계산
        FinalInfluence = (FinalInfluence - 1.0f) * 100.0f;

        AdjustBuildingColor(BuildingActor, FinalInfluence,false);

        // 디버깅 로그
        UE_LOG(LogTemp, Log, TEXT("Building %s has final influence %f%%"), *BuildingActor->GetName(), FinalInfluence);
    }

    // 디버깅용 영향 출력
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
            // 동적 머티리얼 인스턴스 생성
            if (InstMaterial)
            {
                UMaterialInstanceDynamic* DynamicMaterialInstance = UMaterialInstanceDynamic::Create(InstMaterial, this);
                if (DynamicMaterialInstance)
                {
                    // 머티리얼의 Color 파라미터 설정
                    DynamicMaterialInstance->SetVectorParameterValue(FName("Color"), WhiteColor);

                    // 스태틱 메쉬 컴포넌트에 머티리얼 적용
                    Mesh->SetMaterial(0, DynamicMaterialInstance);
                }
            }
        }
    }

    BuildingInfluenceMap.Empty();
}

FLinearColor ADecalManager::GetBuildingColor(float Percentage)
{
    // 퍼센트 값을 채도로 매핑 (0~50 -> 0.5~1.0)
    float Saturation = FMath::GetMappedRangeValueClamped(
        FVector2D(0.0f, 40.0f), FVector2D(50.0f, 255.0f), FMath::Abs(Percentage)
    );

    // 밝기 설정 (고정값 또는 동적으로 설정 가능)
    float Value = 1.0f; // 밝기를 유지

    if (Percentage >= 0)
    {
        float Hue = 160.0f; // 파란색 (Hue: 240)
        //return FLinearColor::MakeFromHSV8(Hue, Saturation * 255, Value * 255);
        return FLinearColor::MakeFromHSV8(160, Saturation, 240);
    }
    else
    {
        float Hue = 0.0f; // 빨간색 (Hue: 0)
        return FLinearColor::MakeFromHSV8(Hue, Saturation, 240);
    }
}

void ADecalManager::AdjustBuildingColor(AActor* BuildingActor, float Percentage, bool SetWhite)
{
    // 색상 계산
    FLinearColor NewColor = GetBuildingColor(Percentage);
    UE_LOG(LogTemp, Log, TEXT("Generated Color: R=%f, G=%f, B=%f, A=%f"), NewColor.R, NewColor.G, NewColor.B, NewColor.A);
    // Static Mesh 컴포넌트 찾기
    UStaticMeshComponent* Mesh = BuildingActor->FindComponentByClass<UStaticMeshComponent>();
    if (Mesh)
    {
        // 동적 머티리얼 인스턴스 생성
        if (InstMaterial)
        {
            UMaterialInstanceDynamic* DynamicMaterialInstance = UMaterialInstanceDynamic::Create(InstMaterial, this);
            if (DynamicMaterialInstance)
            {
                // 머티리얼의 Color 파라미터 설정
                DynamicMaterialInstance->SetVectorParameterValue(FName("Color"), NewColor);

                // 스태틱 메쉬 컴포넌트에 머티리얼 적용
                Mesh->SetMaterial(0, DynamicMaterialInstance);
            }
        }
    }
}

ADecalManager* ADecalManager::GetInstance(UWorld* World)
{
    if (!Instance)
    {
        // 월드에서 매니저 검색
        for (TActorIterator<ADecalManager> It(World); It; ++It)
        {
            Instance = *It;
            break;
        }

        // 매니저가 없으면 새로 생성
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


