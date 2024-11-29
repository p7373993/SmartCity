// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/UIActor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/KismetSystemLibrary.h"


#include "Actor/DecalAct.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UW_smMain.h"
#include "HUD_smMain.h"
#include "ProtoType/Private/UI/DecalActSpawnButton.h"
const double R_Local = 6371000;
const double DEG_TO_RAD_Local = PI / 180.0;
// Sets default values
AUIActor::AUIActor()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create and initialize WidgetComponent
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	static ConstructorHelpers::FClassFinder<UDecalActSpawnButton> WidgetBlueprint(TEXT("/Game/Blueprints/DecalUI.DecalUI_C"));
	if (WidgetBlueprint.Class)
	{
		WidgetComponent->SetWidgetClass(WidgetBlueprint.Class);
		WidgetComponent->SetupAttachment(RootComponent);
		WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
		WidgetComponent->SetWorldRotation(FRotator(0, 0, 0));
		WidgetComponent->SetDrawSize(FVector2D(200, 100));
		WidgetComponent->SetVisibility(true);
	}

	//if (UUW_smMain* MainWidget = UUW_smMain::GetInstance(GetWorld()))
	//{
	//	MainWidget->OnButtonStateChanged.AddDynamic(this, &AUIActor::HandleButtonStateChanged);
	//}
}

void AUIActor::SpawnAct()
{
	Manager.SpawnDecalActor(GetWorld(), this->GetActorLocation(), index);
}

void AUIActor::DeleteAct()
{
	Manager.DeleteActor(index);
}

// Called when the game starts or when spawned
void AUIActor::BeginPlay()
{
	Super::BeginPlay();

	//----------------------
	// UUW_smMain 위젯을 찾음
	bIsUIActive = false;
	SetUIActive(bIsUIActive);
	// 현재 월드의 플레이어 컨트롤러 가져오기
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC && PC->GetHUD())
	{
		// HUD에 추가된 위젯 가져오기
		AHUD_smMain* CustomHUD = Cast<AHUD_smMain>(PC->GetHUD());
		if (CustomHUD && CustomHUD->getHudWidget())
		{
			UUW_smMain* Widget = CustomHUD->getHudWidget();
			if (Widget)
			{
				// Delegate 구독
				Widget->OnButtonStateChanged.AddDynamic(this, &AUIActor::OnUIStateChanged);
			}
		}
	}
	//------------------

	UUserWidget* UserWidget = WidgetComponent->GetUserWidgetObject();
	if (UserWidget)
	{
		UDecalActSpawnButton* DecalWidget = Cast<UDecalActSpawnButton>(UserWidget);
		if (DecalWidget)
		{
			DecalWidget->InitializeWithActor(this);
		}
	}

	float x = 0;
	float y = 0;

	const double baseLatitude = 36.50476937;
	const double baseLongitude = 127.2784241;
	const double scaleX = -8691673.56;
	const double scaleY = -10995829.86;
	double phi0 = 36.50476937 * DEG_TO_RAD_Local;
	double lambda0 = 127.2784241 * DEG_TO_RAD_Local;
	double phi = latitude * DEG_TO_RAD_Local;
	double lambda = longitude * DEG_TO_RAD_Local;
	x = R_Local * (lambda - lambda0) * cos(phi0) * 100 + 13167 - 3040 + 1000 + 1000;
	y = -R_Local * (phi - phi0) * 100 + 3073 + 6597 - 6000 - 200;
	FVector NewLocation(x, y, 50);
	SetActorLocation(NewLocation);
}

// Called every frame
void AUIActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	if (CameraManager)
	{
		FVector CameraLocation = CameraManager->GetCameraLocation();
		FRotator CameraRotation = CameraManager->GetCameraRotation();
		FVector DirectionToWidget = WidgetComponent->GetComponentLocation() - CameraLocation;
		FRotator NewRotation = DirectionToWidget.Rotation();
		WidgetComponent->SetWorldRotation(NewRotation);
	}
}

void AUIActor::HandleButtonStateChanged(bool bIsActive)
{
	if (bIsActive)
	{
		UE_LOG(LogTemp, Log, TEXT("Button activated!"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Button deactivated!"));
	}
}
//ui와 델리게이트로 연결
void AUIActor::OnUIStateChanged(bool bIsActive)
{
	SetUIActive(bIsActive);
}

void AUIActor::SetUIActive(bool bActive)
{
	bIsUIActive = bActive;

	if (bActive)
	{
		UUserWidget* UserWidget = WidgetComponent->GetUserWidgetObject();
		if (UserWidget)
		{
			UDecalActSpawnButton* DecalWidget = Cast<UDecalActSpawnButton>(UserWidget);
			if (DecalWidget)
			{
				DecalWidget->UIVisible();
				DecalActSpawnManager& Temp = DecalActSpawnManager::GetInstance();
				if (Temp.FindActor(index) != nullptr)
				{
					Temp.FindActor(index)->DetectBuildings();
				}
			}
		}
	}
	else
	{
		UUserWidget* UserWidget = WidgetComponent->GetUserWidgetObject();
		if (UserWidget)
		{
			UDecalActSpawnButton* DecalWidget = Cast<UDecalActSpawnButton>(UserWidget);
			if (DecalWidget)
			{
				DecalWidget->UIUnVisible();
				FVector BoxExtent = FVector(100000000000000.0f, 100000000000000.0f, 500000000000000.0f);
				TArray<FOverlapResult> OverlapResults;

				FCollisionQueryParams QueryParams(FName(TEXT("ECC_GameTraceChannel1_Overlap")), true);
				QueryParams.bTraceComplex = true;
				QueryParams.bReturnPhysicalMaterial = false;

				FCollisionObjectQueryParams ObjectQueryParams(ECC_GameTraceChannel1);
				FVector A = FVector(0.0f, 0.0f, 0.0f);
				bool bOverlap = GetWorld()->OverlapMultiByObjectType(
					OverlapResults,
					A,
					FQuat::Identity,
					ObjectQueryParams,
					FCollisionShape::MakeBox(BoxExtent),
					QueryParams
				);

				if (bOverlap)
				{
					for (const FOverlapResult& Result : OverlapResults)
					{
						AActor* OverlappedActor = Result.GetActor();
						if (OverlappedActor)
						{
							TArray<UActorComponent*> Components = OverlappedActor->GetComponentsByClass(UMeshComponent::StaticClass());
							for (UActorComponent* Component : Components)
							{
								UMeshComponent* MeshComponent = Cast<UMeshComponent>(Component);
								if (MeshComponent)
								{
									int32 MaterialCount = MeshComponent->GetNumMaterials();
									for (int32 Index = 0; Index < MaterialCount; ++Index)
									{
										UMaterialInterface* Material = MeshComponent->GetMaterial(Index);
										if (Material)
										{
											UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(Material, OverlappedActor);
											if (DynamicMaterial)
											{
												DynamicMaterial->SetVectorParameterValue(FName("BaseColor"), FLinearColor::White);
												MeshComponent->SetMaterial(Index, DynamicMaterial);
											}
										}
									}
								}
							}
						}
					}
				}

			}
		}
	}

	// 실제 UI 상태를 변경하는 로직 (예: Mesh, Widget 등)
	UE_LOG(LogTemp, Log, TEXT("UI %s"), bIsUIActive ? TEXT("Activated") : TEXT("Deactivated"));
}

