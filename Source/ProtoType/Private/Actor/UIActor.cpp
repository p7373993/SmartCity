// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/UIActor.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "ProtoType/Private/UI/DecalActSpawnButton.h"
const double R = 6371000;
const double DEG_TO_RAD = PI / 180.0;
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
	double phi0 = 36.50476937 * DEG_TO_RAD;
	double lambda0 = 127.2784241 * DEG_TO_RAD;
	double phi = latitude * DEG_TO_RAD;
	double lambda = longitude * DEG_TO_RAD;
	x = R * (lambda - lambda0) * cos(phi0) * 100 + 13167 - 3040 + 1000 + 1000;
	y = -R * (phi - phi0) * 100 + 3073 + 6597 - 6000 - 200;
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

