// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/DecalAct.h"
#include "Kismet/KismetSystemLibrary.h"
// Sets default values
ADecalAct::ADecalAct()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ADecalAct::DetectBuildings()
{
    FVector LandmarkLocation = GetActorLocation(); // 랜드마크 위치
    float Radius = 5000.0f; // 반경 설정 (5000)

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
        AdjustBuildingHeight(Actor); // Building 높이 조정 함수 호출
    }
}

void ADecalAct::AdjustBuildingHeight(AActor* BuildingActor)
{
    // 랜드마크와의 거리 계산
    float Distance = FVector::Dist(GetActorLocation(), BuildingActor->GetActorLocation());

    // 머신러닝 모델에서 제공한 값 (샘플)
    //float PredictedPercent = PredictHeightScale(Distance); // 머신러닝 예측 함수 호출
    float PredictedPercent = 50;

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

FLinearColor ADecalAct::GetBuildingColor(float Percentage)
{
    float Opacity = FMath::Clamp(FMath::Abs(Percentage) / 100.0f, 0.2f, 1.0f); // 불투명도 계산
    if (Percentage > 0)
    {
        // 파란색 계열
        return FLinearColor(0.0f, 0.0f, 1.0f, Opacity); // RGBA (파랑, 불투명도)
    }
    else
    {
        // 빨간색 계열
        return FLinearColor(1.0f, 0.0f, 0.0f, Opacity); // RGBA (빨강, 불투명도)
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

