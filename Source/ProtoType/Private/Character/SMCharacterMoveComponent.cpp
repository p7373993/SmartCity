// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SMCharacterMoveComponent.h"
#include "Character/SMCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"

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
		// Enhanced Input Subsystem�� ������
		UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

		// ��ǲ ���� ���ؽ�Ʈ �߰�
		if (InputSubsystem && InputMapping)
		{
			InputSubsystem->AddMappingContext(InputMapping, 0);
		}

		// ��ǲ �׼� ���ε�
		if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// ��Ŭ�� �׼� ���ε�
			Input->BindAction(LeftClickAction, ETriggerEvent::Started, this, &USMCharacterMoveComponent::OnLeftClick);
			Input->BindAction(LeftClickAction, ETriggerEvent::Completed, this, &USMCharacterMoveComponent::OnLeftClick);

			// ��Ŭ�� �׼� ���ε�
			Input->BindAction(RightClickAction, ETriggerEvent::Started, this, &USMCharacterMoveComponent::OnRightClick);
			Input->BindAction(RightClickAction, ETriggerEvent::Completed, this, &USMCharacterMoveComponent::OnRightClick);

			// �̵� �׼� ���ε�
			Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &USMCharacterMoveComponent::QuaterMove);

			//�巡�� �׼�
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
		// ��Ŭ�� ���¿����� �̵� ó��
		if (Camera)
		{

			float Height = FMath::Clamp(OwningActor->GetActorLocation().Z * 0.005, 0, 500);

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
		// ��Ŭ�� ���¿����� ȭ�� ȸ�� ó��
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

//��Ŭ��
void USMCharacterMoveComponent::OnLeftClick(const FInputActionValue& Value)
{
	bIsLeftClicking = Value.Get<bool>();
}
//��Ŭ��
void USMCharacterMoveComponent::OnRightClick(const FInputActionValue& Value)
{
	bIsRightClicking = Value.Get<bool>();
}
//�ٷ� ���Ʒ�
void USMCharacterMoveComponent::UpDown(const FInputActionValue& Value)
{
	float Height = FMath::Clamp(OwningActor->GetActorLocation().Z * 0.1, 0, 1000);
	float Power = Value.Get<float>();
	FVector CurrentLocation = OwningActor->GetActorLocation();
	FVector NewLocation = CurrentLocation + FVector(0.0f, 0.0f, Power * Height); // �̵� �ӵ��� �����մϴ�.

	OwningActor->SetActorLocation(NewLocation, true); // true�� �浹�� ����Ͽ� ��ġ�� �����մϴ�.
}



