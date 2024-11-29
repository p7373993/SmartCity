// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/UIActor.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UW_smMain.h"
#include "HUD_smMain.h"
#include "ProtoType/Private/UI/DecalActSpawnButton.h"
#include "Manager/DecalManager.h"
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
	// UUW_smMain ������ ã��
	bIsUIActive = false;
	SetUIActive(bIsUIActive);
	// ���� ������ �÷��̾� ��Ʈ�ѷ� ��������
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC && PC->GetHUD())
	{
		// HUD�� �߰��� ���� ��������
		AHUD_smMain* CustomHUD = Cast<AHUD_smMain>(PC->GetHUD());
		if (CustomHUD && CustomHUD->getHudWidget())
		{
			UUW_smMain* Widget = CustomHUD->getHudWidget();
			if (Widget)
			{
				// Delegate ����
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
//ui�� ��������Ʈ�� ����
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

				UWorld* World = GetWorld();
				ADecalManager* DecalManager = ADecalManager::GetInstance(World);
				DecalManager->ClearInfluences();//�ۼ�Ʈ�� ���� ���� ����� machinelearning data delete
			}
		}
	}

	// ���� UI ���¸� �����ϴ� ���� (��: Mesh, Widget ��)
	UE_LOG(LogTemp, Log, TEXT("UI %s"), bIsUIActive ? TEXT("Activated") : TEXT("Deactivated"));
}

