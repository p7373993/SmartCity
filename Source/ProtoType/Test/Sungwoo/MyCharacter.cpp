// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/Classes/Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"


// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);
	Camera->bUsePawnControlRotation = true;

	FRotator CameraRotation = FRotator(-90.0f, 0.0f, 0.0f); // Pitch -90 degrees to look downwards
	Camera->SetRelativeRotation(CameraRotation);

	ensure(Camera);

	bIsLeftClicking = false;
	bIsRightClicking = false;


	if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
	{
		MovementComponent->GravityScale = 0.0f; // 중력 비활성화
		MovementComponent->AirControl = 1.0f;  // 공중에서 이동 조절 활성화
		MovementComponent->JumpZVelocity = 0.0f; // 점프 속도 비활성화
	}
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	MyTCPModule.TCPCunnect();
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMapping, 0);
		}
	}

	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// 좌클릭 액션 바인딩
		Input->BindAction(LeftClickAction, ETriggerEvent::Started, this, &AMyCharacter::OnLeftClick);
		Input->BindAction(LeftClickAction, ETriggerEvent::Completed, this, &AMyCharacter::OnLeftClick);

		// 우클릭 액션 바인딩
		Input->BindAction(RightClickAction, ETriggerEvent::Started, this, &AMyCharacter::OnRightClick);
		Input->BindAction(RightClickAction, ETriggerEvent::Completed, this, &AMyCharacter::OnRightClick);

		// 이동 액션 바인딩
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyCharacter::QuaterMove);

		//드래그 액션
		Input->BindAction(UpDownAction, ETriggerEvent::Triggered, this, &AMyCharacter::UpDown);

	}

}

void AMyCharacter::RayCast(const FVector& StartLocation, const FVector& EndLocation, int32 FloorInfo)
{
	UObject* WorldContextObject = GetWorld();
	if (!WorldContextObject) return;

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	bool bHit = WorldContextObject->GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams);

	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 5.0f, 0, 1.0f);

	if (bHit)
	{
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		AActor* HitActor = HitResult.GetActor();

		if (HitActor)
		{
			UStaticMeshComponent* HitStaticMesh = Cast<UStaticMeshComponent>(HitActor->GetComponentByClass(UStaticMeshComponent::StaticClass()));
			if (HitStaticMesh)
			{

				UMaterialInterface* Material = HitStaticMesh->GetMaterial(0);

				if (Material)
				{
					UMaterialInstanceDynamic* DynamicMaterialInstance = UMaterialInstanceDynamic::Create(Material, this);
					if (DynamicMaterialInstance)
					{
						// 머티리얼 속성 설정 (예: 색상)
						FLinearColor NewColor = FLinearColor::Red;
						DynamicMaterialInstance->SetVectorParameterValue(FName("Color"), NewColor);

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

		DrawDebugLine(GetWorld(), HitResult.Location + FVector(0, 0, 10000.0f), HitResult.Location, FColor::Green, false, 5.0f, 0, 1.0f);
	}
}


void AMyCharacter::QuaterMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (bIsLeftClicking)
	{
		ensure(Camera);
		// 좌클릭 상태에서의 이동 처리
		if (Camera)
		{

			float Height = FMath::Clamp(GetActorLocation().Z * 0.005, 0, 500);

			FVector CameraForward = Camera->GetForwardVector();
			FVector CameraRight = Camera->GetRightVector();

			FVector MoveDirection = CameraForward * MovementVector.Y + CameraRight * MovementVector.X;
			MoveDirection.Z = 0.0f;

			FVector CurrentLocation = GetActorLocation();
			FVector NewLocation = CurrentLocation + (MoveDirection * Height);

			SetActorLocation(NewLocation, true);

			//AddMovementInput(MoveDirection, MovementVector.Size());


			FString DebugMessage = FString::Printf(TEXT("Move with Left Clicks (%f:%f)"), MovementVector.X, MovementVector.Y);
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, DebugMessage);
			}
		}
	}

	if (bIsRightClicking)
	{
		// 우클릭 상태에서의 화면 회전 처리
		APlayerController* PlayerController = Cast<APlayerController>(Controller);
		if (PlayerController)
		{
			FRotator NewRotation = PlayerController->GetControlRotation();
			NewRotation.Yaw += MovementVector.X;
			NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch + MovementVector.Y, -89.0f, 0.0f);
			PlayerController->SetControlRotation(NewRotation);

			FString DebugMessage = FString::Printf(TEXT("Look with Right Click (%f:%f)"), MovementVector.X, MovementVector.Y);
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, DebugMessage);
			}
		}
	}

}

void AMyCharacter::OnLeftClick(const FInputActionValue& Value)
{
	bIsLeftClicking = Value.Get<bool>();
	FString DebugMessage = FString::Printf(TEXT("Left Click %s"), bIsLeftClicking ? TEXT("Pressed") : TEXT("Released"));
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, DebugMessage);
	}
}

void AMyCharacter::OnRightClick(const FInputActionValue& Value)
{
	bIsRightClicking = Value.Get<bool>();
	FString DebugMessage = FString::Printf(TEXT("Right Click %s"), bIsRightClicking ? TEXT("Pressed") : TEXT("Released"));
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, DebugMessage);
	}
}

void AMyCharacter::UpDown(const FInputActionValue& Value)
{
	float Height =FMath::Clamp(GetActorLocation().Z * 0.1,0,1000);
	float Power = Value.Get<float>();
	FVector CurrentLocation = GetActorLocation();
	FVector NewLocation = CurrentLocation + FVector(0.0f, 0.0f, Power * Height); // 이동 속도를 조정합니다.

	SetActorLocation(NewLocation, true); // true는 충돌을 고려하여 위치를 설정합니다.

	FString DebugMessage = FString::Printf(TEXT("[Drag Move]Current Height (%f)"), float(GetActorLocation().Z));
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, DebugMessage);
	}


}

FViewLocation AMyCharacter::GetCornerPoints()
{
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (!PlayerController)
	{
		return FViewLocation();

	}
	FViewLocation ViewLocation;

	// 화면 모서리 좌표
	const FVector2D ScreenCorners[4] = {
		FVector2D(0, 0), // Left-up
		FVector2D(0, GEngine->GameViewport->Viewport->GetSizeXY().Y), // Left-down
		FVector2D(GEngine->GameViewport->Viewport->GetSizeXY().X, 0), // Right-up
		FVector2D(GEngine->GameViewport->Viewport->GetSizeXY().X, GEngine->GameViewport->Viewport->GetSizeXY().Y) // Right-down
	};

	FHitResult HitResult;
	FVector WorldLocation;
	FVector WorldDirection;

	// 각 화면 모서리에 대해 처리
	for (int32 i = 0; i < 4; i++)
	{
		if (PlayerController->DeprojectScreenPositionToWorld(ScreenCorners[i].X, ScreenCorners[i].Y, WorldLocation, WorldDirection))
		{
			// 바닥과의 교차점을 찾기 위해 라인 트레이스 수행
			FVector Start = WorldLocation;
			FVector End = WorldLocation + (WorldDirection * 1000000.0f); // 충분히 긴 거리

			// 라인 트레이스, 바닥 오브젝트만 대상으로
			if (GetWorld()->LineTraceSingleByObjectType(
				HitResult,
				Start,
				End,
				FCollisionObjectQueryParams(ECollisionChannel::ECC_WorldStatic)
			))
			{
				// 히트 결과를 저장
				switch (i)
				{
				case 0 :
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

				// 디버그 라인 그리기
				DrawDebugLine(GetWorld(), Start, HitResult.Location, FColor::Red, false, 5.0f, 0, 1.0f);
				DrawDebugPoint(GetWorld(), HitResult.Location, 10.0f, FColor::Green, false, 5.0f);
			}
		}
	}
	GetPoint(ViewLocation.LeftUp, ViewLocation.LeftDown, ViewLocation.RightUp, ViewLocation.RightDown);
	return ViewLocation;
}

void AMyCharacter::TempRayCast()
{
	FVector StartLocation = GetActorLocation();
	FVector Direction = GetActorForwardVector();
	FVector EndLocation = StartLocation + (Direction * 1000.0f);
	RayCast(StartLocation, EndLocation);
}

void AMyCharacter::GetPoint(FVector2D LU, FVector2D LD, FVector2D RU, FVector2D RD)
{

}



