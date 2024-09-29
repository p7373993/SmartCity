// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SMCharacterMoveComponent.h"
#include "Character/SMCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
// Sets default values for this component's properties
USMCharacterMoveComponent::USMCharacterMoveComponent()
{
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> MappingContextRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Sungwoo/Input/IMC_Default.IMC_Default'"));
	if (MappingContextRef.Object)
	{
		InputMapping = MappingContextRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> MoveActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Sungwoo/Input/IA_Move.IA_Move'"));
	if (MoveActionRef.Object)
	{
		MoveAction = MoveActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> LeftClickActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Sungwoo/Input/IA_LeftClick.IA_LeftClick'"));
	if (LeftClickActionRef.Object)
	{
		LeftClickAction = LeftClickActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> RightClickActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Sungwoo/Input/IA_RightClick.IA_RightClick'"));
	if (RightClickActionRef.Object)
	{
		RightClickAction = RightClickActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> UpDownActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Sungwoo/Input/IA_UpDown.IA_UpDown'"));
	if (UpDownActionRef.Object)
	{
		UpDownAction = UpDownActionRef.Object;
	}

	bIsLeftClicking = false;
	bIsRightClicking = false;

}


// Called when the game starts
void USMCharacterMoveComponent::BeginPlay()
{
	Super::BeginPlay();
	OwningActor = Cast<ASMCharacter>(GetOwner());

	PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		// Enhanced Input Subsystem을 가져옴
		UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

		// 인풋 매핑 컨텍스트 추가
		if (InputSubsystem && InputMapping)
		{
			InputSubsystem->AddMappingContext(InputMapping, 0);
		}

		// 인풋 액션 바인딩
		if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// 좌클릭 액션 바인딩
			Input->BindAction(LeftClickAction, ETriggerEvent::Started, this, &USMCharacterMoveComponent::OnLeftClick);
			Input->BindAction(LeftClickAction, ETriggerEvent::Completed, this, &USMCharacterMoveComponent::OnLeftClick);

			// 우클릭 액션 바인딩
			Input->BindAction(RightClickAction, ETriggerEvent::Started, this, &USMCharacterMoveComponent::OnRightClick);
			Input->BindAction(RightClickAction, ETriggerEvent::Completed, this, &USMCharacterMoveComponent::OnRightClick);

			// 이동 액션 바인딩
			Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &USMCharacterMoveComponent::QuaterMove);

			//드래그 액션
			Input->BindAction(UpDownAction, ETriggerEvent::Triggered, this, &USMCharacterMoveComponent::UpDown);
		}
	}



	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = true;
		PlayerController->bEnableClickEvents = true;
		PlayerController->bEnableMouseOverEvents = true;

	}
}

void USMCharacterMoveComponent::GetActorTag()
{
	if (PlayerController)
	{
		if (bIsLeftClicking)
		{
			PlayerController->GetMousePosition(MouseLocation.X, MouseLocation.Y);
			FInputModeGameOnly InputMode;
			PlayerController->SetInputMode(InputMode);

			PlayerController->bShowMouseCursor = false;

		}
		else
		{
			PlayerController->bShowMouseCursor = true;
			PlayerController->bEnableClickEvents = true;
			PlayerController->bEnableMouseOverEvents = true;

			PlayerController->SetMouseLocation(MouseLocation.X, MouseLocation.Y);
		}
	}

	FVector2D MousePosition;
	if (GEngine && GEngine->GameViewport)
	{
		if (GEngine->GameViewport->GetMousePosition(MousePosition))
		{
			UE_LOG(LogTemp, Warning, TEXT("Mouse Position: X=%f, Y=%f"),
				MousePosition.X, MousePosition.Y);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to get mouse position."));
		}
	}


	float MouseX, MouseY;
	MouseX = MousePosition.X;
	MouseY = MousePosition.Y;
	if (true)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		FVector WorldLocation, WorldDirection;
		PC->DeprojectScreenPositionToWorld(MousePosition.X, MousePosition.Y, WorldLocation, WorldDirection);
		FVector End = WorldLocation + (WorldDirection * 1000000000000000000000000000000000000.f);
		FHitResult HitResult;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(PC->GetPawn());
		bool bHit = GetWorld()->LineTraceSingleByChannel(
			HitResult,
			WorldLocation,
			End,
			ECC_Visibility,
			CollisionParams
		);
		if (bHit)
		{
			DrawDebugLine(GetWorld(), WorldLocation, HitResult.Location, FColor::Green, false, 5.f, 0, 1.f);
			FString DebugMessage = FString::Printf(TEXT("Hit: %s"), *HitResult.GetActor()->GetName());
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, DebugMessage);
		}
		else
		{
			DrawDebugLine(GetWorld(), WorldLocation, End, FColor::Red, false, 5.f, 0, 1.f);

			FString DebugMessage = TEXT("No hit");
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, DebugMessage);
		}
		if (bHit && HitResult.GetActor())
		{
			AActor* HitActor = HitResult.GetActor();
			UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitActor->GetName());
			if (HitActor->Tags.Num() > 0)
			{
				for (const FName& Tag : HitActor->Tags)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, Tag.ToString());
				}
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No Tags Found"));
			}
		}
	}
}

void USMCharacterMoveComponent::QuaterMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (bIsLeftClicking)
	{


		UCameraComponent* Camera = OwningActor->FindComponentByClass<UCameraComponent>();
		ensure(Camera);
		// 좌클릭 상태에서의 이동 처리
		if (Camera)
		{

			float Height = FMath::Clamp(OwningActor->GetActorLocation().Z * 0.005, 0, 2000);

			FVector CameraForward = Camera->GetForwardVector();
			FVector CameraRight = Camera->GetRightVector();

			FVector MoveDirection = CameraForward * MovementVector.Y + CameraRight * MovementVector.X;
			MoveDirection.Z = 0.0f;

			FVector CurrentLocation = OwningActor->GetActorLocation();
			FVector NewLocation = CurrentLocation + (MoveDirection * Height);

			OwningActor->SetActorLocation(NewLocation, true);


		}
	}

	if (bIsRightClicking)
	{

		// 우클릭 상태에서의 화면 회전 처리

		if (PlayerController)
		{

			FRotator NewRotation = PlayerController->GetControlRotation();
			NewRotation.Yaw += MovementVector.X;
			NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch + MovementVector.Y, -89.0f, -50.0f);
			PlayerController->SetControlRotation(NewRotation);

			FString DebugMessage = FString::Printf(TEXT("Look with Right Click (%f:%f)"), MovementVector.X, MovementVector.Y);
		}
	}
}

//좌클릭
void USMCharacterMoveComponent::OnLeftClick(const FInputActionValue& Value)
{
	bIsLeftClicking = Value.Get<bool>();
	GetActorTag();
}
//우클릭
void USMCharacterMoveComponent::OnRightClick(const FInputActionValue& Value)
{
	bIsRightClicking = Value.Get<bool>();

	if (PlayerController)
	{
		if (bIsRightClicking)
		{


			PlayerController->GetMousePosition(MouseLocation.X, MouseLocation.Y);
			FInputModeGameOnly InputMode;
			PlayerController->SetInputMode(InputMode);
			PlayerController->bShowMouseCursor = false;
		}
		else
		{
			PlayerController->bShowMouseCursor = true;
			PlayerController->bEnableClickEvents = true;
			PlayerController->bEnableMouseOverEvents = true;

			
			PlayerController->SetMouseLocation(MouseLocation.X, MouseLocation.Y);
		}
	}
}
//휠로 위아래
void USMCharacterMoveComponent::UpDown(const FInputActionValue& Value)
{
	float Height = FMath::Clamp(OwningActor->GetActorLocation().Z * 0.5, 0, 10000);
	float Power = Value.Get<float>();
	FVector CurrentLocation = OwningActor->GetActorLocation();
	FVector NewLocation = CurrentLocation + FVector(0.0f, 0.0f, Power * Height); // 이동 속도를 조정합니다.

	OwningActor->SetActorLocation(NewLocation, true); // true는 충돌을 고려하여 위치를 설정합니다.
}
