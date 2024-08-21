#include "ProtoType/RayCastTest/RayCastTest001.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "ProtoType/ClientModule/TCPModule.h"


#include <cmath>

const double R = 6371000;
const double DEG_TO_RAD = PI / 180.0;

void ARayCastTest001::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

ARayCastTest001::ARayCastTest001()
{
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialFinder(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Sungwoo/M_DynamicInst.M_DynamicInst'"));

    if (MaterialFinder.Succeeded())
    {
        InstMaterial = MaterialFinder.Object;
    }
}

void ARayCastTest001::GetPoint(FVector2D LU, FVector2D LD, FVector2D RU, FVector2D RD)
{
    //for (const auto& item : MyTCPModule.GetAPData(/* Input std::vector XY = (latitude1, longitude1, latitude2, longitude2, latitude3, longitude3, latitude4, longitude4) 각각 위도 경도를 vector에 추가해서 보내기 */ ))
    //{
    //    /* item.latitude와 item.longitude를 X, Y좌표로 바꾸는 공식*/
    //        RayCast(GetWorld(), FVector(X, Y, 1000000), FVector(X, Y, -100));
    //}

    std::vector<float> lA;

    double latitude;
    double longitude;
    XYTolatLong(LU.X, LU.Y, latitude, longitude);//이순서대로 넣어야 됨 
    lA.push_back(latitude);
    lA.push_back(longitude);

    XYTolatLong(RU.X, RU.Y, latitude, longitude);
    lA.push_back(latitude);
    lA.push_back(longitude);

    XYTolatLong(RD.X, RD.Y, latitude, longitude);
    lA.push_back(latitude);
    lA.push_back(longitude);

    XYTolatLong(LD.X, LD.Y, latitude, longitude);
    lA.push_back(latitude);
    lA.push_back(longitude);

    //lA.push_back(36.513564);

    //lA.push_back(126.225986);

    //lA.push_back(35.513564);

    //lA.push_back(128.225986);

    //lA.push_back(37.513564);

    //lA.push_back(126.225986);

    //lA.push_back(35.513564);

    //lA.push_back(128.225986);
    for (const auto& item : MyTCPModule.GetAPData(lA)) {
        UE_LOG(LogTemp, Warning, TEXT("%d"), item.ApartIndex);



        double Latitude;
        double Longitude;

        Latitude = item.latitude;
        Longitude = item.longitude;
        double x;
        double y;
        latLongToXY(Latitude, Longitude, x, y);
        RayCast(FVector(x, y, 10000000000), FVector(x, y, -1000));
    }
}

void ARayCastTest001::RayCast(const FVector& StartLocation, const FVector& EndLocation,int32 FloorInfo)
{

    // 머티리얼 속성 설정 (예: 색상)
    int32 MaxFloorInfo =20 ;//최고점 값
    FLinearColor NewColor = GetSpectrumColor(FloorInfo, MaxFloorInfo);

    //if(FloorInfo<7) NewColor = FLinearColor::Green;
    //else if(FloorInfo < 14) NewColor = FLinearColor::Blue; 
    //else if (FloorInfo < 20) NewColor = FLinearColor::Red;
    //else NewColor = FLinearColor::Black;

    
    UObject* WorldContextObject = GetWorld();
    DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 50.0f, 0, 100.0f);
    if (!WorldContextObject) return;
    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    bool bHit = WorldContextObject->GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams);
    if (bHit)
    {
        UPrimitiveComponent* HitComponent = HitResult.GetComponent();
        AActor* HitActor = HitResult.GetActor();

        if (HitActor)
        {
            if (HitComponent->GetCollisionObjectType() == ECC_GameTraceChannel1)//건물 맞을떄
            {
                ChangeBuildingMaterial(HitResult, NewColor);
            }
            else//바닥맞을떄
            {
                float SphereRadius = 2000.0f;
                FHitResult SphereHitResult;
                FCollisionQueryParams SphereQueryParams;
                SphereQueryParams.AddIgnoredActor(this);
                bool bResult = GetWorld()->SweepSingleByChannel(
                    SphereHitResult,
                    HitResult.ImpactPoint,
                    HitResult.ImpactPoint + FVector(0, 0, 100.0f),
                    FQuat::Identity,
                    ECollisionChannel::ECC_GameTraceChannel3,
                    FCollisionShape::MakeSphere(SphereRadius),
                    SphereQueryParams

                );

                if (bResult) {
                    HitComponent = SphereHitResult.GetComponent();
                    if (HitComponent->GetCollisionObjectType() == ECC_GameTraceChannel1)
                    {
                        ChangeBuildingMaterial(SphereHitResult, NewColor);
                    }




#if ENABLE_DRAW_DEBUG//디버그 모드에서만 디버그 캡슐 그리도록
                    FVector TraceVec = FVector(0, 0, 100.f);
                    FVector Center = HitResult.ImpactPoint + TraceVec * 0.5f;
                    float HalfHeight = 50.f;
                    FColor DrawColor = (HitComponent->GetCollisionObjectType() == ECC_GameTraceChannel1) ? FColor::Green : FColor::Red;
                    float DebugLifeTime = 50.0f;

                    DrawDebugCapsule(GetWorld(),
                        Center,
                        HalfHeight,
                        SphereRadius,//radius
                        FQuat::Identity,
                        DrawColor,
                        false,
                        DebugLifeTime
                    );

#endif // ENABLE_DRAW_DEBUG//디버그 모드에서만 디버그 캡슐 그리도록
                    if (bResult)
                    {

                    }
                }
            }
        }


        DrawDebugLine(GetWorld(), HitResult.Location + FVector(0, 0, 10000.0f), HitResult.Location, FColor::Green, false, 5.0f, 0, 1.0f);
        
    }
}
void ARayCastTest001::ChangeBuildingMaterial(FHitResult& HitResult, FLinearColor InNewColor)
{
    UPrimitiveComponent* HitComponent = HitResult.GetComponent();
    AActor* HitActor = HitResult.GetActor();
    if (HitActor)
    {
        if (HitComponent->GetCollisionObjectType() == ECC_GameTraceChannel1)
        {
            UStaticMeshComponent* HitStaticMesh = Cast<UStaticMeshComponent>(HitActor->GetComponentByClass(UStaticMeshComponent::StaticClass()));

            if (HitStaticMesh)
            {

                if (InstMaterial)
                {
                    UMaterialInstanceDynamic* DynamicMaterialInstance = UMaterialInstanceDynamic::Create(InstMaterial, this);
                    if (DynamicMaterialInstance)
                    {
                        DynamicMaterialInstance->SetVectorParameterValue(FName("Color"), InNewColor);

                        // 스태틱 메쉬 컴포넌트에 머티리얼 적용
                        HitStaticMesh->SetMaterial(0, DynamicMaterialInstance);

                        UE_LOG(LogTemp, Warning, TEXT("Material applied successfully."));
                    }
                    else
                    {
                        UE_LOG(LogTemp, Warning, TEXT("Failed to create dynamic material instance."));
                    }
                }
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("No static mesh component found on the hit actor."));
            }
        }
    }

}
void ARayCastTest001::ChangeMaterialRGB(USkeletalMeshComponent* SkeletalMesh, FName BoneName, FLinearColor NewColor)
{
    if (!SkeletalMesh) return;
    int32 BoneIndex = SkeletalMesh->GetBoneIndex(BoneName);
    if (BoneIndex == INDEX_NONE) return;
    FLinearColor RedColor = FLinearColor(1.0f, 0.0f, 0.0f, 1.0f);
    TArray<UMaterialInterface*> Materials = SkeletalMesh->GetMaterials();
    for (UMaterialInterface* Material : Materials)
    {
        if (Material)
        {
            UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(Material, SkeletalMesh);
            if (DynamicMaterial)
            {
                //DynamicMaterial->SetBlendMode(BLEND_Opaque);
                DynamicMaterial->SetVectorParameterValue(FName("BaseColor"), RedColor);
                DynamicMaterial->SetScalarParameterValue(FName("Opacity"), 1.0f);
                SkeletalMesh->SetMaterial(0, DynamicMaterial);
            }
        }
    }
}

FLinearColor ARayCastTest001::GetSpectrumColor(float Value, int32 MaxValue)//색 value로 스펙트럼으로 구현 파>빨
{
    // Value should be in the range [0, 100]
   // Map Value to the wavelength range [400, 700] nm
    float Wavelength = FMath::Lerp(400.0f, 700.0f, float(Value / MaxValue));

    float RGB_R = 0.0f;
    float RGB_G = 0.0f;
    float RGB_B = 0.0f;

    if (Wavelength >= 400.0f && Wavelength <= 440.0f) {
        RGB_R = -(Wavelength - 440.0f) / (440.0f - 400.0f);
        RGB_G = 0.0f;
        RGB_B = 1.0f;
    }
    else if (Wavelength > 440.0f && Wavelength <= 490.0f) {
        RGB_R = 0.0f;
        RGB_G = (Wavelength - 440.0f) / (490.0f - 440.0f);
        RGB_B = 1.0f;
    }
    else if (Wavelength > 490.0f && Wavelength <= 510.0f) {
        RGB_R = 0.0f;
        RGB_G = 1.0f;
        RGB_B = -(Wavelength - 510.0f) / (510.0f - 490.0f);
    }
    else if (Wavelength > 510.0f && Wavelength <= 580.0f) {
        RGB_R = (Wavelength - 510.0f) / (580.0f - 510.0f);
        RGB_G = 1.0f;
        RGB_B = 0.0f;
    }
    else if (Wavelength > 580.0f && Wavelength <= 645.0f) {
        RGB_R = 1.0f;
        RGB_G = -(Wavelength - 645.0f) / (645.0f - 580.0f);
        RGB_B = 0.0f;
    }
    else if (Wavelength > 645.0f && Wavelength <= 700.0f) {
        RGB_R = 1.0f;
        RGB_G = 0.0f;
        RGB_B = 0.0f;
    }

    // Apply intensity factor
    float Intensity = 1.0f;
    if (Wavelength > 700.0f || Wavelength < 400.0f) {
        Intensity = 0.0f;
    }
    else if (Wavelength > 645.0f) {
        Intensity = 0.3f + 0.7f * (700.0f - Wavelength) / (700.0f - 645.0f);
    }
    else if (Wavelength < 420.0f) {
        Intensity = 0.3f + 0.7f * (Wavelength - 400.0f) / (420.0f - 400.0f);
    }

    return FLinearColor(RGB_R * Intensity, RGB_G * Intensity, RGB_B * Intensity);
}

void ARayCastTest001::latLongToXY(double latitude, double longitude, double& x, double& y)
{
    const double baseLatitude = 36.50476937;
    const double baseLongitude = 127.2784241;



    const double scaleX = -8691673.56;
    const double scaleY = -10995829.86;

    double phi0 = 36.50476937 * DEG_TO_RAD;
    double lambda0 = 127.2784241 * DEG_TO_RAD;
    double phi = latitude * DEG_TO_RAD;
    double lambda = longitude * DEG_TO_RAD;

    x = R * (lambda - lambda0) * cos(phi0) * 100 + 13167 - 3040 + 1000 + 1000;
    y = -R * (phi - phi0) * 100 + 3073 + 6597 - 6000 - 200;
}

void ARayCastTest001::XYTolatLong(double x, double y, double& latitude, double& longitude)
{
    const double baseLatitude = 36.50476937;
    const double baseLongitude = 127.2784241;

    const double scaleX = -8691673.56;
    const double scaleY = -10995829.86;
    
    double phi0 = baseLatitude * DEG_TO_RAD;
    double lambda0 = baseLongitude * DEG_TO_RAD; 
    double phi = phi0 - (y - 3470) / (R * 100);
    double lambda = lambda0 + (x - 13127) / (R * cos(phi0) * 100);

    latitude = phi / DEG_TO_RAD;
    longitude = lambda / DEG_TO_RAD;
}