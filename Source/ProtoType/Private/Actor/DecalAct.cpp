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
    FVector LandmarkLocation = GetActorLocation(); // ���帶ũ ��ġ
    float Radius = 50000.0f; // �ݰ� ���� (5000)

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
        float Distance = FVector::Dist(GetActorLocation(), Actor->GetActorLocation());
        float PredictedPercent = 30;// �ӽŷ��� ���� �� ��������

        // ���� ����
        AdjustBuildingColor(Actor, PredictedPercent);

        // (����) ���� ����
        AdjustBuildingHeight(Actor, PredictedPercent);

    }
}

void ADecalAct::AdjustBuildingHeight(AActor* BuildingActor, float PredictedPercent)
{
    // ���帶ũ���� �Ÿ� ���
    float Distance = FVector::Dist(GetActorLocation(), BuildingActor->GetActorLocation());

    // �ӽŷ��� �𵨿��� ������ �� (����)
    //float PredictedPercent = PredictHeightScale(Distance); // �ӽŷ��� ���� �Լ� ȣ��

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

void ADecalAct::AdjustBuildingColor(AActor* BuildingActor, float Percentage)
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

FLinearColor ADecalAct::GetBuildingColor(float Percentage)
{
    // �ۼ�Ʈ ���� ä���� ���� (0~50 -> 0.5~1.0)
    float Saturation = FMath::GetMappedRangeValueClamped(
        FVector2D(0.0f, 50.0f), FVector2D(150.0f, 255.0f), FMath::Abs(Percentage)
    );

    // ��� ���� (������ �Ǵ� �������� ���� ����)
    float Value = 1.0f; // ��⸦ ����

    if (Percentage > 0)
    {
        float Hue = 160.0f; // �Ķ��� (Hue: 240)
        //return FLinearColor::MakeFromHSV8(Hue, Saturation * 255, Value * 255);
        return FLinearColor::MakeFromHSV8(160, Saturation,  240);
    }
    else
    {
        float Hue = 0.0f; // ������ (Hue: 0)
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

