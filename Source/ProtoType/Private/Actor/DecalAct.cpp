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
    TSet<AActor*> UniqueOverlappingActors; // 중복 방지를 위한 Set
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1)); // 대상 오브젝트 타입

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
            ObjectTypes, // Object Types를 기준으로 탐지
            nullptr, // 특정 클래스 제한 없음
            {}, // 필터링할 액터
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

 

    //FVector LandmarkLocation = GetActorLocation(); // 랜드마크 위치
    //float Radius = 200000.0f; // 반경 설정 (5000)

    //// 부딪히는 액터들을 저장할 배열
    //TArray<AActor*> OverlappingActors;

    //// Object Type 설정
    //TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    //ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1)); // Building 채널 추가

    //// 콜리전 실행
    //UKismetSystemLibrary::SphereOverlapActors(
    //    GetWorld(),
    //    LandmarkLocation,
    //    Radius,
    //    ObjectTypes, // Object Types를 기준으로 탐지
    //    nullptr, // 특정 클래스 제한 없음
    //    {}, // 필터링할 액터
    //    OverlappingActors
    //);

    //// 탐지된 액터 처리
    //TMap<float, float> CurrentData = GetLandmarkData();

    //for (AActor* Actor : OverlappingActors)
    //{
    //    UStaticMeshComponent* StaticMeshComp = Actor->FindComponentByClass<UStaticMeshComponent>();
    //    float Distance;
    //    float PredictedPercent = 0;
    //    // StaticMesh와의 거리 계산
    //    if (StaticMeshComp)
    //    {
    //        // StaticMesh의 World 위치 가져오기
    //        FVector MeshLocation = StaticMeshComp->GetComponentLocation();
    //        // StaticMesh와의 거리 계산
    //        Distance = FVector::Dist(GetActorLocation(), MeshLocation);
    //        UE_LOG(LogTemp, Log, TEXT("Mesh Location : %f,%f,%f"), MeshLocation.X, MeshLocation.Y, MeshLocation.Z);
    //    }
    //    for (const auto& Pair : CurrentData)
    //    {
    //        float UnrealDistance = Pair.Key; // JSON 거리 값을 언리얼 거리 값으로 변환 (0.2 -> 20000)

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

    // 기본 높이 조정 로직
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
        FVector2D(0.0f, 40.0f), FVector2D(50.0f, 255.0f), FMath::Abs(Percentage)
    );

    // 밝기 설정 (고정값 또는 동적으로 설정 가능)
    float Value = 1.0f; // 밝기를 유지

    if (Percentage >= 0)
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
            return TMap<ELandMarkType, TMap<float, float>>(); // 빈 데이터 반환
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

