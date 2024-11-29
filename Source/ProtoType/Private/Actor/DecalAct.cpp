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

#include "ProtoType/Private/ClientModule/TCPModule.h"

#include "ProtoType/Global/Structs.h"

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


    SelectedLandMark = ELandMarkType::Hotel;
}

void ADecalAct::DetectBuildings()
{
    TSet<AActor*> UniqueOverlappingActors; // �ߺ� ������ ���� Set
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1)); // ��� ������Ʈ Ÿ��

    FVector LandmarkLocation = GetActorLocation();
    TArray<AActor*> OverlappingActors;
    TMap<float, float> CurrentData = GetLandmarkData();
    for (const auto& Pair : CurrentData)
    {
        float Radius = Pair.Key;
        float PredictedPercent = Pair.Value;

        UKismetSystemLibrary::SphereOverlapActors(
            GetWorld(),
            LandmarkLocation,
            Radius,
            ObjectTypes, // Object Types�� �������� Ž��
            nullptr, // Ư�� Ŭ���� ���� ����
            {}, // ���͸��� ����
            OverlappingActors
        );

        UE_LOG(LogTemp, Error, TEXT("Distance: %f, Percent : %f "), Radius, PredictedPercent);



        for (AActor* Actor : OverlappingActors)
        {

            if (!UniqueOverlappingActors.Contains(Actor))
            {
                UniqueOverlappingActors.Add(Actor);

                AdjustBuildingColor(Actor, PredictedPercent);//color

                //AdjustBuildingHeight(Actor, PredictedPercent);//height
            }


        }




    }

 

    //FVector LandmarkLocation = GetActorLocation(); // ���帶ũ ��ġ
    //float Radius = 200000.0f; // �ݰ� ���� (5000)

    //// �ε����� ���͵��� ������ �迭
    //TArray<AActor*> OverlappingActors;

    //// Object Type ����
    //TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    //ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1)); // Building ä�� �߰�

    //// �ݸ��� ����
    //UKismetSystemLibrary::SphereOverlapActors(
    //    GetWorld(),
    //    LandmarkLocation,
    //    Radius,
    //    ObjectTypes, // Object Types�� �������� Ž��
    //    nullptr, // Ư�� Ŭ���� ���� ����
    //    {}, // ���͸��� ����
    //    OverlappingActors
    //);

    //// Ž���� ���� ó��
    //TMap<float, float> CurrentData = GetLandmarkData();

    //for (AActor* Actor : OverlappingActors)
    //{
    //    UStaticMeshComponent* StaticMeshComp = Actor->FindComponentByClass<UStaticMeshComponent>();
    //    float Distance;
    //    float PredictedPercent = 0;
    //    // StaticMesh���� �Ÿ� ���
    //    if (StaticMeshComp)
    //    {
    //        // StaticMesh�� World ��ġ ��������
    //        FVector MeshLocation = StaticMeshComp->GetComponentLocation();
    //        // StaticMesh���� �Ÿ� ���
    //        Distance = FVector::Dist(GetActorLocation(), MeshLocation);
    //        UE_LOG(LogTemp, Log, TEXT("Mesh Location : %f,%f,%f"), MeshLocation.X, MeshLocation.Y, MeshLocation.Z);
    //    }
    //    for (const auto& Pair : CurrentData)
    //    {
    //        float UnrealDistance = Pair.Key; // JSON �Ÿ� ���� �𸮾� �Ÿ� ������ ��ȯ (0.2 -> 20000)

    //        if (FMath::IsNearlyEqual(UnrealDistance, Distance, 10000.0f))
    //        {
    //            PredictedPercent = Pair.Value;
    //            break;
    //        }

    //    }

    //    AdjustBuildingColor(Actor, PredictedPercent);
    //    AdjustBuildingHeight(Actor, PredictedPercent);
    //}
}


void ADecalAct::AdjustBuildingHeight(AActor* BuildingActor, float PredictedPercent)
{

    // �⺻ ���� ���� ����
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
        FVector2D(0.0f, 40.0f), FVector2D(50.0f, 255.0f), FMath::Abs(Percentage)
    );

    // ��� ���� (������ �Ǵ� �������� ���� ����)
    float Value = 1.0f; // ��⸦ ����

    if (Percentage >= 0)
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
    /*
    SSelectorType selector;
    ELandMarkType Temp;
    switch(Index)
    {
    case 1:
        selector.Elemental[0] = 1;
        Temp = ELandMarkType::Bridge;
    case 2:
        selector.Elemental[0] = 2;
        Temp = ELandMarkType::Stadium;
    case 3:
        selector.Elemental[0] = 3;
        Temp = ELandMarkType::CityMuseum;
    case 4:
        selector.Elemental[0] = 4;
        Temp = ELandMarkType::NationalMuseum;
    case 5:
        selector.Elemental[0] = 5;
        Temp = ELandMarkType::Hotel;
    default:
        break;
    }
    TCPModule& MyTCPModule = TCPModule::GetInstance();
    selector.Elemental[1] = 10000;
    selector.Elemental[2] = 1000;
    std::vector<float>Data = MyTCPModule.GetDecalDistance(selector.Elemental,2);
    float Temp3 = 0;
    TMap<float, float> Temp4;
    for(auto Temp2 : Data)
    {
        Temp3 = Temp3 + selector.Elemental[2];
        Temp4.Add(Temp3, Temp2);
    }
    MachineLearningData.Add(Temp, Temp4);*/
    MachineLearningData = LoadMachineLearningData(JsonPath);
    if (MachineLearningData.Contains(SelectedLandMark))
    {
        UE_LOG(LogTemp, Log, TEXT("Data loaded for selected landmark."));
    }
    DetectBuildings();
}

TMap<ELandMarkType, TMap<float, float>> ADecalAct::LoadMachineLearningData(const FString& FilePath)
{
    TMap<ELandMarkType, TMap<float, float>> ProcessedData;
    UE_LOG(LogTemp, Log, TEXT("Attempting to load JSON file from: %s"), *FilePath);


    FString JsonContent;
    if (FFileHelper::LoadFileToString(JsonContent, *FilePath))
    {
        TSharedPtr<FJsonObject> JsonObject;
        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonContent);

        if (!FFileHelper::LoadFileToString(JsonContent, *FilePath))
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to load JSON file from path: %s"), *FilePath);
            return TMap<ELandMarkType, TMap<float, float>>(); // �� ������ ��ȯ
        }
        else
        {
            UE_LOG(LogTemp, Log, TEXT("Successfully loaded JSON file: %s"), *FilePath);
        }

        if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
        {
            TMap<FString, ELandMarkType> LandmarkNameMap = {
                {"Bridge", ELandMarkType::Bridge},
                {"Stadium", ELandMarkType::Stadium},
                {"CityMuseum", ELandMarkType::CityMuseum},
                {"NationalMuseum", ELandMarkType::NationalMuseum},
                {"Hotel", ELandMarkType::Hotel}
            };

            for (const auto& LandmarkPair : JsonObject->Values)
            {
                FString LandmarkName = LandmarkPair.Key;
                if (LandmarkNameMap.Contains(LandmarkName))
                {
                    ELandMarkType LandmarkType = LandmarkNameMap[LandmarkName];
                    TSharedPtr<FJsonObject> LandmarkData = LandmarkPair.Value->AsObject();

                    if (LandmarkData.IsValid())
                    {
                        TMap<float, float> DistanceToPercentage;

                        for (const auto& DistancePair : LandmarkData->Values)
                        {
                            float JsonDistance = FCString::Atof(*DistancePair.Key);
                            float UnrealDistance = JsonDistance * 100000.0f;
                            float Percentage = DistancePair.Value->AsNumber();

                            DistanceToPercentage.Add(UnrealDistance, Percentage);
                        }

                        ProcessedData.Add(LandmarkType, DistanceToPercentage);
                    }
                }
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load JSON file: %s"), *FilePath);
    }

    return ProcessedData;
}

TMap<float, float> ADecalAct::GetLandmarkData() const
{
    if (MachineLearningData.Contains(SelectedLandMark))
    {
        return MachineLearningData[SelectedLandMark];
    }
    return {};
}



// Called every frame
void ADecalAct::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

