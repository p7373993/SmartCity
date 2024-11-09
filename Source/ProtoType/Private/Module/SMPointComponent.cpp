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



	//���� Ÿ��
	CurrentType = EVisibleType::None;
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
		// ��������Ʈ�� ���ε�
		FWorldDelegates::LevelAddedToWorld.AddUObject(this, &USMPointComponent::OnLevelLoaded);

		FWorldDelegates::PostApplyLevelOffset.AddUObject(this, &USMPointComponent::OnLevelLoadedWithOffset);

	}
	FTimerHandle UnusedHandle;
	OwningActor->GetWorldTimerManager().SetTimer(UnusedHandle, this, &USMPointComponent::LevelPoint, 0.5f, false);

	FTimerHandle RangeChangeHandle;
	OwningActor->GetWorldTimerManager().SetTimer(RangeChangeHandle, this, &USMPointComponent::RangeChange, 10.f, true);
}

FViewLocation USMPointComponent::GetCornerPoints()
{
	APlayerController* PlayerController = Cast<APlayerController>(OwningActor->Controller);
	if (!PlayerController)
	{
		return FViewLocation();
	}

	FViewLocation ViewLocation = FViewLocation();

	// ȭ�� �𼭸� ��ǥ
	const FVector2D ScreenCorners[4] = {
		FVector2D(0, 0), // Left-up
		FVector2D(0, GEngine->GameViewport->Viewport->GetSizeXY().Y), // Left-down
		FVector2D(GEngine->GameViewport->Viewport->GetSizeXY().X, 0), // Right-up
		FVector2D(GEngine->GameViewport->Viewport->GetSizeXY().X, GEngine->GameViewport->Viewport->GetSizeXY().Y) // Right-down
	};

	FHitResult HitResult;
	FVector WorldLocation;
	FVector WorldDirection;

	// �� ȭ�� �𼭸��� ���� ó��
	for (int32 i = 0; i < 4; i++)
	{
		if (PlayerController->DeprojectScreenPositionToWorld(ScreenCorners[i].X, ScreenCorners[i].Y, WorldLocation, WorldDirection))
		{
			// �ٴڰ��� �������� ã�� ���� ���� Ʈ���̽� ����
			FVector Start = WorldLocation;
			FVector End = WorldLocation + (WorldDirection * 1000000.0f); // ����� �� �Ÿ�

			// ���� Ʈ���̽�, �ٴ� ������Ʈ�� �������
			if (GetWorld()->LineTraceSingleByObjectType(
				HitResult,
				Start,
				End,
				FCollisionObjectQueryParams(ECollisionChannel::ECC_WorldStatic)
			))
			{
				// ��Ʈ ����� ����
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

				// ����� ���� �׸���
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


	// ���� �ٿ�带 ã�� ���� ALevelBounds ���͸� �˻�
	ALevelBounds* LevelBoundsActor = nullptr;
	ULevel* InLevel = GetWorld()->GetCurrentLevel();
	for (AActor* Actor : InLevel->Actors)
	{
		LevelBoundsActor = Cast<ALevelBounds>(Actor);
		if (LevelBoundsActor)
		{
			break;  // ALevelBounds ���͸� ã�����Ƿ� ���� Ż��
		}
	}

	if (LevelBoundsActor)
	{
		// ALevelBounds���� ���(Bounds) ��������
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

void USMPointComponent::RangeChange()
{

	if (OwningActor)
	{
		FVector CurrentLocation = OwningActor->GetActorLocation();

		float Offset;
		Offset = FMath::Clamp(CurrentLocation.Z * 20,50000,500000);//���̶� ����
		FVector2D BottomLeft = FVector2D(CurrentLocation.X - Offset, CurrentLocation.Y - Offset);
		FVector2D BottomRight = FVector2D(CurrentLocation.X + Offset, CurrentLocation.Y - Offset);
		FVector2D TopLeft = FVector2D(CurrentLocation.X - Offset, CurrentLocation.Y + Offset);
		FVector2D TopRight = FVector2D(CurrentLocation.X + Offset, CurrentLocation.Y + Offset);
		FViewLocation Location = FViewLocation(TopLeft, BottomLeft, BottomRight, TopRight);

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

	GetAPDataThread();

	/*if (MyTCPModule.IsInUse == false)
	{
		Async(EAsyncExecution::Thread, [this]() { });
	}*/
}

 void USMPointComponent::GetAPDataThread()
 {
	 float TemplA[20] = { lA[0],lA[1],lA[2],lA[3],lA[4],lA[5],lA[6],lA[7],lA[8],lA[9],lA[10],lA[11],lA[12],lA[13],lA[14],lA[15],lA[16],lA[17],lA[18],lA[19]};
	 std::vector<APData> TempAPData = MyTCPModule.GetAPData(TemplA, 0);
	 std::vector<SaleData> TempSaleData = MyTCPModule.GetSaleData(TemplA, 0);
	 Async(EAsyncExecution::TaskGraphMainThread, [&, TempAPData, TempSaleData]() {
		 for (const auto& APDatas : TempAPData)
		 {
			 int Price = 0;
			 long long DealIndex = 0;
			 for (const auto& SaleDatas : TempSaleData)
			 {
				 
				 if (SaleDatas.ApartIndex == APDatas.ApartIndex)
				 {
					 if (DealIndex < SaleDatas.articleNo)
					 {
						 DealIndex = SaleDatas.articleNo;
						 Price = SaleDatas.dealOrWarrantPrc;
						 UE_LOG(LogTemp, Warning, TEXT("001The value of number is: %d"), Price);
					 }
				 }
			 }
			 double Latitude;
			 double Longitude;

			 Latitude = APDatas.latitude;
			 Longitude = APDatas.longitude;

			 double x;
			 double y;
			 latLongToXY(Latitude, Longitude, x, y);
			 UE_LOG(LogTemp, Warning, TEXT("002The value of number is: %d"), Price);
			 RayCast(FVector(x, y, 10000000000), FVector(x, y, -1000), APDatas, Price);
		 }
		 });
 }

void USMPointComponent::RayCast(const FVector& StartLocation, const FVector& EndLocation, const APData& Data, int Price)
{
	bool IsNone = false;
	float value = static_cast<float>(Price);
	switch (CurrentType)
	{
	case EVisibleType::None:
		IsNone = true;
		break;
	case EVisibleType::Price:
		value = Price;
		break;
	case EVisibleType::Floor:
		value = Price;
		break;
	default:
		break;
	}

	UE_LOG(LogTemp, Warning, TEXT("003The value of number is: %d"), Price);
	UE_LOG(LogTemp, Warning, TEXT("004The value of number is: %f"), value);
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


//#if ENABLE_DRAW_DEBUG//����� ��忡���� ����� ĸ�� �׸�����
//			FColor LineColor = (HitComponent->GetCollisionObjectType() == ECC_GameTraceChannel1) ? FColor::Green : FColor::Red;
//			// ���� Ʈ���̽� �ð�ȭ
//			DrawDebugLine(
//				WorldContextObject->GetWorld(), // ���� ����
//				StartLocation,                  // ���� ��ġ
//				EndLocation,                    // �� ��ġ
//				LineColor,                      // ������ ����
//				false,                          // ���� ǥ�� ���� (false�� ���� �ð� ���ȸ� ǥ�õ�)
//				100.0f,                           // ǥ�� �ð� (��)
//				0,                              // ���� �켱 ����
//				100.0f                            // ���� �β�
//			);
//#endif // ENABLE_DRAW_DEBUG//����� ��忡���� ����� ĸ�� �׸�����

			if (HitComponent->GetCollisionObjectType() == ECC_GameTraceChannel1)//�ǹ� ������
			{
				ChangeBuildingMaterial(HitResult, NewColor);

				if (!HitActor->Tags.Contains("Target"))
				{
					bool IsTaged = false;
					for (const FName& Tag : HitActor->Tags)
					{
						IsTaged = true;
					}
					if (!IsTaged)
					{
						FString TagString = FString::Printf(TEXT("%d"), Data.ApartIndex);
						HitActor->Tags.Add(FName(*TagString));
					}

				}

			}
			else//�ٴڸ�����
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
#if ENABLE_DRAW_DEBUG//����� ��忡���� ����� ĸ�� �׸�����
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
					AActor* SphereHitActor = SphereHitResult.GetActor();
					if (!SphereHitActor->Tags.Contains("Target"))
					{
						bool IsTaged = false;
						for (const FName& Tag : SphereHitActor->Tags)
						{
							IsTaged = true;
						}
						if (!IsTaged)
						{
							FString TagString = FString::Printf(TEXT("%d"), Data.ApartIndex);
							SphereHitActor->Tags.Add(FName(*TagString));
						}

					}

#endif // ENABLE_DRAW_DEBUG//����� ��忡���� ����� ĸ�� �׸�����

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

						// ����ƽ �޽� ������Ʈ�� ��Ƽ���� ����
						HitStaticMesh->SetMaterial(0, DynamicMaterialInstance);

					}

				}
			}

		}
	}
}

FLinearColor USMPointComponent::GetSpectrumColor(float Value)
{
	float RGB_R = 0.0f;
	float RGB_G = 0.0f;
	float RGB_B = 0.0f;

	if (Value >= 0.0f && Value <= 30000.0f) {
		// Black to Red
		RGB_R = Value / 30000.0f; // R goes from 0 to 1
		RGB_G = 0.0f; // G remains 0
		RGB_B = 0.0f; // B remains 0
	}
	else if (Value > 30000.0f && Value <= 70000.0f) {
		// Yellow to Green
		RGB_R = 1.0f - (Value - 30000.0f) / (70000.0f - 30000.0f); // R decreases from 1 to 0
		RGB_G = (Value - 30000.0f) / (70000.0f - 30000.0f); // G increases from 0 to 1
		RGB_B = 0.0f; // B remains 0
	}
	else if (Value > 70000.0f && Value <= 150000.0f) {
		// Green to Cyan
		RGB_R = 0.0f; // R remains 0
		RGB_G = 1.0f - (Value - 70000.0f) / (150000.0f - 70000.0f); // G decreases from 1 to 0
		RGB_B = (Value - 70000.0f) / (150000.0f - 70000.0f); // B increases from 0 to 1
	}
	else if (Value > 150000.0f) {
		// White
		RGB_R = 1.0f; // R = 1
		RGB_G = 1.0f; // G = 1
		RGB_B = 1.0f; // B = 1
	}

	// Apply intensity factor (optional, but you may want to keep it)
	float Intensity = 1.0f; // Adjust if you want to manage brightness
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







