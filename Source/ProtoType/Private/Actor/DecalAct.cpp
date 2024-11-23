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
    FVector LandmarkLocation = GetActorLocation(); // ���帶ũ ��ġ
    float Radius = 5000.0f; // �ݰ� ���� (5000)

    // �ε����� ���͵��� ������ �迭
    TArray<AActor*> OverlappingActors;

    // Object Type ����
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1)); // Building ä�� �߰�

    // �ݸ��� ����
    UKismetSystemLibrary::SphereOverlapActors(
        GetWorld(),
        LandmarkLocation,
        Radius,
        ObjectTypes, // Object Types�� �������� Ž��
        nullptr, // Ư�� Ŭ���� ���� ����
        {}, // ���͸��� ����
        OverlappingActors
    );

    // Ž���� ���� ó��
    for (AActor* Actor : OverlappingActors)
    {
        AdjustBuildingHeight(Actor); // Building ���� ���� �Լ� ȣ��
    }
}

void ADecalAct::AdjustBuildingHeight(AActor* BuildingActor)
{
    // ���帶ũ���� �Ÿ� ���
    float Distance = FVector::Dist(GetActorLocation(), BuildingActor->GetActorLocation());

    // �ӽŷ��� �𵨿��� ������ �� (����)
    //float PredictedPercent = PredictHeightScale(Distance); // �ӽŷ��� ���� �Լ� ȣ��
    float PredictedPercent = 50;

    // �⺻ ���� ���� ����
    float BaseHeight = 300.0f; // �⺻ ���� (���� ��)
    float NewHeightScale = 1.0f + PredictedPercent / 10.0f;

    // Static Mesh Component ã�Ƽ� ���� ����
    UStaticMeshComponent* Mesh = BuildingActor->FindComponentByClass<UStaticMeshComponent>();
    if (Mesh)
    {
        FVector NewScale = Mesh->GetComponentScale();
        NewScale.Z = NewHeightScale; // ���� �����ϸ� ����
        Mesh->SetWorldScale3D(NewScale);
    }
}

FLinearColor ADecalAct::GetBuildingColor(float Percentage)
{
    float Opacity = FMath::Clamp(FMath::Abs(Percentage) / 100.0f, 0.2f, 1.0f); // ������ ���
    if (Percentage > 0)
    {
        // �Ķ��� �迭
        return FLinearColor(0.0f, 0.0f, 1.0f, Opacity); // RGBA (�Ķ�, ������)
    }
    else
    {
        // ������ �迭
        return FLinearColor(1.0f, 0.0f, 0.0f, Opacity); // RGBA (����, ������)
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

