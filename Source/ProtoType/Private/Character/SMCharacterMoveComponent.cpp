// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SMCharacterMoveComponent.h"
#include "Character/SMCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"

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

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		// Enhanced Input SubsystemÀ» °¡Á®¿È
		UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

		// ÀÎÇ² ¸ÅÇÎ ÄÁÅØ½ºÆ® Ãß°¡
		if (InputSubsystem && InputMapping)
		{
			InputSubsystem->AddMappingContext(InputMapping, 0);
		}

		// ÀÎÇ² ¾×¼Ç ¹ÙÀÎµù
		if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// ÁÂÅ¬¸¯ ¾×¼Ç ¹ÙÀÎµù
			Input->BindAction(LeftClickAction, ETriggerEvent::Started, this, &USMCharacterMoveComponent::OnLeftClick);
			Input->BindAction(LeftClickAction, ETriggerEvent::Completed, this, &USMCharacterMoveComponent::OnLeftClick);

			// ¿ìÅ¬¸¯ ¾×¼Ç ¹ÙÀÎµù
			Input->BindAction(RightClickAction, ETriggerEvent::Started, this, &USMCharacterMoveComponent::OnRightClick);
			Input->BindAction(RightClickAction, ETriggerEvent::Completed, this, &USMCharacterMoveComponent::OnRightClick);

			// ÀÌµ¿ ¾×¼Ç ¹ÙÀÎµù
			Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &USMCharacterMoveComponent::QuaterMove);

			//µå·¡±× ¾×¼Ç
			Input->BindAction(UpDownAction, ETriggerEvent::Triggered, this, &USMCharacterMoveComponent::UpDown);
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
		// ÁÂÅ¬¸¯ »óÅÂ¿¡¼­ÀÇ ÀÌµ¿ Ã³¸®
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
		// ¿ìÅ¬¸¯ »óÅÂ¿¡¼­ÀÇ È­¸é È¸Àü Ã³¸®
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController)
		{
			FRotator NewRotation = PlayerController->GetControlRotation();
			NewRotation.Yaw += MovementVector.X;
			NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch + MovementVector.Y, -89.0f, 0.0f);
			PlayerController->SetControlRotation(NewRotation);

			FString DebugMessage = FString::Printf(TEXT("Look with Right Click (%f:%f)"), MovementVector.X, MovementVector.Y);
		}
	}
}

//ÁÂÅ¬¸¯
void USMCharacterMoveComponent::OnLeftClick(const FInputActionValue& Value)
{
	bIsLeftClicking = Value.Get<bool>();
	FVector2D MousePosition;
	if (GEngine && GEngine->GameViewport)
	{
		// 마우스 위치를 저장할 변수 선언
		

		// GameViewportClient를 통해 마우스 위치 가져오기
		if (GEngine->GameViewport->GetMousePosition(MousePosition))
		{
			// 마우스 좌표 출력
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
		FVector WorldLocation, WorldDirection;
		FVector End = WorldLocation + (WorldDirection * 10000.f);
		FHitResult HitResult;
		FCollisionQueryParams CollisionParams;

		bool bHit = GetWorld()->LineTraceSingleByChannel(
			HitResult,
			WorldLocation,
			End,
			ECC_Visibility,
			CollisionParams
		);
		if (bHit && HitResult.GetActor())
		{
			AActor* HitActor = HitResult.GetActor();
			UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitActor->GetName());
			if (HitActor->Tags.Num() > 0)
			{
				for (const FName& Tag : HitActor->Tags)
				{
					// 태그를 출력 (화면에 출력)
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
//¿ìÅ¬¸¯
void USMCharacterMoveComponent::OnRightClick(const FInputActionValue& Value)
{
	bIsRightClicking = Value.Get<bool>();
}
//ÈÙ·Î À§¾Æ·¡
void USMCharacterMoveComponent::UpDown(const FInputActionValue& Value)
{
	float Height = FMath::Clamp(OwningActor->GetActorLocation().Z * 0.5, 0, 10000);
	float Power = Value.Get<float>();
	FVector CurrentLocation = OwningActor->GetActorLocation();
	FVector NewLocation = CurrentLocation + FVector(0.0f, 0.0f, Power * Height); // ÀÌµ¿ ¼Óµµ¸¦ Á¶Á¤ÇÕ´Ï´Ù.

	OwningActor->SetActorLocation(NewLocation, true); // true´Â Ãæµ¹À» °í·ÁÇÏ¿© À§Ä¡¸¦ ¼³Á¤ÇÕ´Ï´Ù.
}



