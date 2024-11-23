// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/DecalAct.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "Components/StaticMeshComponent.h"

#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Json.h"
#include "JsonUtilities.h"

#include <cmath>
// Sets default values
ADecalAct::ADecalAct()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialFinder(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Sungwoo/M_DynamicInst.M_DynamicInst'"));

    if (MaterialFinder.Succeeded())
    {
        InstMaterial = MaterialFinder.Object;
    }

}

void ADecalAct::DetectBuildings()
{
    FVector LandmarkLocation = GetActorLocation(); // 랜드마크 위치
    float Radius = 50000.0f; // 반경 설정 (5000)

    // 부딪히는 액터들을 저장할 배열
    TArray<AActor*> OverlappingActors;

    // Object Type 설정
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1)); // Building 채널 추가

    // 콜리전 실행
    UKismetSystemLibrary::SphereOverlapActors(
        GetWorld(),
        LandmarkLocation,
        Radius,
        ObjectTypes, // Object Types를 기준으로 탐지
        nullptr, // 특정 클래스 제한 없음
        {}, // 필터링할 액터
        OverlappingActors
    );

    // 탐지된 액터 처리
    for (AActor* Actor : OverlappingActors)
    {
        float Distance = FVector::Dist(GetActorLocation(), Actor->GetActorLocation());
        float PredictedPercent = 30;// 머신러닝 예측 값 가져오기

        // 색상 적용
        AdjustBuildingColor(Actor, PredictedPercent);

        // (선택) 높이 조정
        AdjustBuildingHeight(Actor, PredictedPercent);

    }
}

void ADecalAct::AdjustBuildingHeight(AActor* BuildingActor, float PredictedPercent)
{
    // 랜드마크와의 거리 계산
    float Distance = FVector::Dist(GetActorLocation(), BuildingActor->GetActorLocation());

    // 머신러닝 모델에서 제공한 값 (샘플)
    //float PredictedPercent = PredictHeightScale(Distance); // 머신러닝 예측 함수 호출

    // 기본 높이 조정 로직
    float BaseHeight = 300.0f; // 기본 높이 (예제 값)
    float NewHeightScale = 1.0f + PredictedPercent / 10.0f;

    // Static Mesh Component 찾아서 높이 조정
    UStaticMeshComponent* Mesh = BuildingActor->FindComponentByClass<UStaticMeshComponent>();
    if (Mesh)
    {
        FVector NewScale = Mesh->GetComponentScale();
        NewScale.Z = NewHeightScale; // 높이 스케일만 조정
        Mesh->SetWorldScale3D(NewScale);
    }
}

void ADecalAct::AdjustBuildingColor(AActor* BuildingActor, float Percentage)
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

FLinearColor ADecalAct::GetBuildingColor(float Percentage)
{
    // 퍼센트 값을 채도로 매핑 (0~50 -> 0.5~1.0)
    float Saturation = FMath::GetMappedRangeValueClamped(
        FVector2D(0.0f, 50.0f), FVector2D(150.0f, 255.0f), FMath::Abs(Percentage)
    );

    // 밝기 설정 (고정값 또는 동적으로 설정 가능)
    float Value = 1.0f; // 밝기를 유지

    if (Percentage > 0)
    {
        float Hue = 160.0f; // 파란색 (Hue: 240)
        //return FLinearColor::MakeFromHSV8(Hue, Saturation * 255, Value * 255);
        return FLinearColor::MakeFromHSV8(160, Saturation,  240);
    }
    else
    {
        float Hue = 0.0f; // 빨간색 (Hue: 0)
        return FLinearColor::MakeFromHSV8(Hue, Saturation, 240);
    }
}

// Called when the game starts or when spawned
void ADecalAct::BeginPlay()
{
	Super::BeginPlay();
    DetectBuildings();
}



// Called every frame
void ADecalAct::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

