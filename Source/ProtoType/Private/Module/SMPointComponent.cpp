// Fill out your copyright notice in the Description page of Project Settings.


#include "Module/SMPointComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"
#include "Data/SMVisibleData.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "Engine/World.h"
#include "Engine/Level.h"
#include "GameFramework/Actor.h"
#include "Engine/LevelBounds.h"
#include "TimerManager.h"


#include<stdio.h>
#include <cmath>

const double R = 6371000;
const double DEG_TO_RAD = PI / 180.0;

float lA[20];

// Sets default values for this component's properties
USMPointComponent::USMPointComponent()
{
	static ConstructorHelpers::FObjectFinder<USMVisibleData> PriceDataRef(TEXT("/Script/ProtoType.SMVisibleData'/Game/Sungwoo/Data/VIsible/DA_Price.DA_Price'"));
	if(PriceDataRef.Object)
	{
		TypeControlManager.Add(EVisibleType::Price, PriceDataRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<USMVisibleData> FloorDataRef(TEXT("/Script/ProtoType.SMVisibleData'/Game/Sungwoo/Data/VIsible/DA_Floor.DA_Floor'"));
	if (FloorDataRef.Object)
	{
		TypeControlManager.Add(EVisibleType::Floor, FloorDataRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialFinder(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Sungwoo/M_DynamicInst.M_DynamicInst'"));

	if (MaterialFinder.Succeeded())
	{
		InstMaterial = MaterialFinder.Object;
	}



	//ÇöÀç Å¸ÀÔ
	CurrentType = EVisibleType::Floor;
	SetCurrentTypeData();

}


// Called when the game starts
void USMPointComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	OwningActor = Cast<ACharacter>(GetOwner());
	MyTCPModule.TCPCunnect();

	UWorld* World = GetWorld();
	if (World)
	{
		// µ¨¸®°ÔÀÌÆ®¿¡ ¹ÙÀÎµù
		FWorldDelegates::LevelAddedToWorld.AddUObject(this, &USMPointComponent::OnLevelLoaded);

		FWorldDelegates::PostApplyLevelOffset.AddUObject(this, &USMPointComponent::OnLevelLoadedWithOffset);

	}
	FTimerHandle UnusedHandle;
	OwningActor->GetWorldTimerManager().SetTimer(UnusedHandle, this, &USMPointComponent::OnceAfterBeginPlay, 0.5f, false);
}

FViewLocation USMPointComponent::GetCornerPoints()
{
	APlayerController* PlayerController = Cast<APlayerController>(OwningActor->Controller);
	if (!PlayerController)
	{
		return FViewLocation();
	}

	FViewLocation ViewLocation = FViewLocation();

	// È­¸é ¸ð¼­¸® ÁÂÇ¥
	const FVector2D ScreenCorners[4] = {
		FVector2D(0, 0), // Left-up
		FVector2D(0, GEngine->GameViewport->Viewport->GetSizeXY().Y), // Left-down
		FVector2D(GEngine->GameViewport->Viewport->GetSizeXY().X, 0), // Right-up
		FVector2D(GEngine->GameViewport->Viewport->GetSizeXY().X, GEngine->GameViewport->Viewport->GetSizeXY().Y) // Right-down
	};

	FHitResult HitResult;
	FVector WorldLocation;
	FVector WorldDirection;

	// °¢ È­¸é ¸ð¼­¸®¿¡ ´ëÇØ Ã³¸®
	for (int32 i = 0; i < 4; i++)
	{
		if (PlayerController->DeprojectScreenPositionToWorld(ScreenCorners[i].X, ScreenCorners[i].Y, WorldLocation, WorldDirection))
		{
			// ¹Ù´Ú°úÀÇ ±³Â÷Á¡À» Ã£±â À§ÇØ ¶óÀÎ Æ®·¹ÀÌ½º ¼öÇà
			FVector Start = WorldLocation;
			FVector End = WorldLocation + (WorldDirection * 1000000.0f); // ÃæºÐÈ÷ ±ä °Å¸®

			// ¶óÀÎ Æ®·¹ÀÌ½º, ¹Ù´Ú ¿ÀºêÁ§Æ®¸¸ ´ë»óÀ¸·Î
			if (GetWorld()->LineTraceSingleByObjectType(
				HitResult,
				Start,
				End,
				FCollisionObjectQueryParams(ECollisionChannel::ECC_WorldStatic)
			))
			{
				// È÷Æ® °á°ú¸¦ ÀúÀå
				switch (i)
				{
				case 0:
					ViewLocation.LeftUp = FVector2D(HitResult.Location);
					break;
				case 1:
					ViewLocation.LeftDown = FVector2D(HitResult.Location);
					break;
				case 2:
					ViewLocation.RightUp = FVector2D(HitResult.Location);
					break;
				case 3:
					ViewLocation.RightDown = FVector2D(HitResult.Location);
					break;
				default:
					break;
				}

				// µð¹ö±× ¶óÀÎ ±×¸®±â
				DrawDebugLine(GetWorld(), Start, HitResult.Location, FColor::Red, false, 5.0f, 0, 1.0f);
				DrawDebugPoint(GetWorld(), HitResult.Location, 10.0f, FColor::Green, false, 5.0f);
			}
		}
	}
	GetPoint(ViewLocation);
	return ViewLocation;
}

void USMPointComponent::LevelPoint()
{
	// ·¹º§ ¹Ù¿îµå¸¦ Ã£±â À§ÇØ ALevelBounds ¾×ÅÍ¸¦ °Ë»ö
	ALevelBounds* LevelBoundsActor = nullptr;
	ULevel* InLevel = GetWorld()->GetCurrentLevel();
	for (AActor* Actor : InLevel->Actors)
	{
		LevelBoundsActor = Cast<ALevelBounds>(Actor);
		if (LevelBoundsActor)
		{
			break;  // ALevelBounds ¾×ÅÍ¸¦ Ã£¾ÒÀ¸¹Ç·Î ·çÇÁ Å»Ãâ
		}
	}

	if (LevelBoundsActor)
	{
		// ALevelBounds¿¡¼­ °æ°è(Bounds) °¡Á®¿À±â
		FBox LevelBounds = LevelBoundsActor->GetComponentsBoundingBox();

		FVector Min = LevelBounds.Min;
		FVector Max = LevelBounds.Max;

		FVector2D BottomLeft = FVector2D(Min.X*0.8, Min.Y*0.8);
		FVector2D BottomRight = FVector2D(Max.X*0.8, Min.Y*0.8);
		FVector2D TopLeft = FVector2D(Min.X*0.8, Max.Y*0.8);
		FVector2D TopRight = FVector2D(Max.X*0.8, Max.Y*0.8);
		FViewLocation Location = FViewLocation(TopLeft, BottomLeft, BottomRight,TopRight);

		GetPoint(Location);
	}

}

void USMPointComponent::GetPoint(FViewLocation& InLocation)
{
	double latitude;
	double longitude;


	int i = 0;

	for (const FVector2D& Vec : InLocation.GetArray())
	{

		XYTolatLong(Vec.X, Vec.Y, latitude, longitude);
		lA[i] = latitude;
		lA[(++i)] = longitude;
		i++;
	}

	if (MyTCPModule.IsInUse == false)
	{
		Async(EAsyncExecution::Thread, [this]() {GetAPDataThread(); });
	}
}

 void USMPointComponent::GetAPDataThread()
 {
	 float TemplA[20] = { lA[0],lA[1],lA[2],lA[3],lA[4],lA[5],lA[6],lA[7],lA[8],lA[9],lA[10],lA[11],lA[12],lA[13],lA[14],lA[15],lA[16],lA[17],lA[18],lA[19]};
	 std::vector<APData> TempAPData = MyTCPModule.GetAPData(TemplA);
	 Async(EAsyncExecution::TaskGraphMainThread, [&, TempAPData]() {
		 for (const auto& APDatas : TempAPData)
		 {
			 UE_LOG(LogTemp, Warning, TEXT("%d"), APDatas.ApartIndex);



			 double Latitude;
			 double Longitude;

			 Latitude = APDatas.latitude;
			 Longitude = APDatas.longitude;

			 double x;
			 double y;
			 latLongToXY(Latitude, Longitude, x, y);
			 RayCast(FVector(x, y, 10000000000), FVector(x, y, -1000), APDatas);
		 }
		 });
 }

void USMPointComponent::RayCast(const FVector& StartLocation, const FVector& EndLocation, const APData& Data)
{
	bool IsNone = false;
	float value = 0;
	switch (CurrentType)
	{
	case EVisibleType::None:
		IsNone = true;
		break;
	case EVisibleType::Price:
		//value = Data.price;
		break;
	case EVisibleType::Floor:
		value = Data.floorInfo;
		break;
	default:
		break;
	}
	FLinearColor NewColor = IsNone ? FLinearColor::White :GetSpectrumColor(value);

	UObject* WorldContextObject = GetWorld();
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
			// 태그를 추가하는 코드
			if (!HitActor->Tags.Contains("Target"))
			{
				HitActor->Tags.Add("Target");
			}

//#if ENABLE_DRAW_DEBUG//µð¹ö±× ¸ðµå¿¡¼­¸¸ µð¹ö±× Ä¸½¶ ±×¸®µµ·Ï
//			FColor LineColor = (HitComponent->GetCollisionObjectType() == ECC_GameTraceChannel1) ? FColor::Green : FColor::Red;
//			// ¶óÀÎ Æ®·¹ÀÌ½º ½Ã°¢È­
//			DrawDebugLine(
//				WorldContextObject->GetWorld(), // ¿ùµå ÂüÁ¶
//				StartLocation,                  // ½ÃÀÛ À§Ä¡
//				EndLocation,                    // ³¡ À§Ä¡
//				LineColor,                      // ¶óÀÎÀÇ »ö»ó
//				false,                          // ¿µ±¸ Ç¥½Ã ¿©ºÎ (false¸é ÀÏÁ¤ ½Ã°£ µ¿¾È¸¸ Ç¥½ÃµÊ)
//				100.0f,                           // Ç¥½Ã ½Ã°£ (ÃÊ)
//				0,                              // ±íÀÌ ¿ì¼± ¼øÀ§
//				100.0f                            // ¼±ÀÇ µÎ²²
//			);
//#endif // ENABLE_DRAW_DEBUG//µð¹ö±× ¸ðµå¿¡¼­¸¸ µð¹ö±× Ä¸½¶ ±×¸®µµ·Ï

			if (HitComponent->GetCollisionObjectType() == ECC_GameTraceChannel1)//°Ç¹° ¸ÂÀ»‹š
			{
				ChangeBuildingMaterial(HitResult, NewColor);
			}
			else//¹Ù´Ú¸ÂÀ»‹š
			{
				float SphereRadius = 2000.0f;
				FHitResult SphereHitResult;
				FCollisionQueryParams SphereQueryParams;
				SphereQueryParams.AddIgnoredActor(OwningActor);
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
#if ENABLE_DRAW_DEBUG//µð¹ö±× ¸ðµå¿¡¼­¸¸ µð¹ö±× Ä¸½¶ ±×¸®µµ·Ï
					FVector TraceVec = FVector(0, 0, 100.f);
					FVector Center = HitResult.ImpactPoint + TraceVec * 0.5f;
					float HalfHeight = 50.f;
					FColor DrawColor = (HitComponent->GetCollisionObjectType() == ECC_GameTraceChannel1) ? FColor::Green : FColor::Red;
					float DebugLifeTime = 5.0f;

					DrawDebugCapsule(GetWorld(),
						Center,
						HalfHeight,
						SphereRadius,//radius
						FQuat::Identity,
						DrawColor,
						false,
						DebugLifeTime
					);

#endif // ENABLE_DRAW_DEBUG//µð¹ö±× ¸ðµå¿¡¼­¸¸ µð¹ö±× Ä¸½¶ ±×¸®µµ·Ï

				}
				
			}
		}
	}
}


void USMPointComponent::latLongToXY(double latitude, double longitude, double& x, double& y)
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

void USMPointComponent::XYTolatLong(double x, double y, double& latitude, double& longitude)
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

void USMPointComponent::OnceAfterBeginPlay()
{
	LevelPoint();
}

void USMPointComponent::ChangeBuildingMaterial(FHitResult& HitResult, FLinearColor InNewColor)
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

						// ½ºÅÂÆ½ ¸Þ½¬ ÄÄÆ÷³ÍÆ®¿¡ ¸ÓÆ¼¸®¾ó Àû¿ë
						HitStaticMesh->SetMaterial(0, DynamicMaterialInstance);

					}

				}
			}

		}
	}
}

FLinearColor USMPointComponent::GetSpectrumColor(float Value)
{
	float Wavelength = FMath::Lerp(400.0f, 700.0f, (Value - MinValue) / (MaxValue - MinValue));

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

void USMPointComponent::SetCurrentTypeData()
{
	if (CurrentType == EVisibleType::None)
	{
		MaxValue = 0.f;
		MinValue = 0.f;
			return;
	}

	MaxValue = TypeControlManager[CurrentType]->MaxValue;
	MinValue = TypeControlManager[CurrentType]->MinValue;

}

void USMPointComponent::TempChangeType(EVisibleType NewType)
{
	CurrentType = NewType;
	SetCurrentTypeData();
	LevelPoint();
}

void USMPointComponent::OnLevelLoaded(ULevel* InLevel, UWorld* InWorld)
{

}

void USMPointComponent::OnLevelLoadedWithOffset(ULevel* InLeve, UWorld* InWorld, const FVector& Offset, bool Inbool)
{

}







