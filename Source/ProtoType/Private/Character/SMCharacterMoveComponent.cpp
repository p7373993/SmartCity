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

	PlayerController = GetWorld()->GetFirstPlayerController();
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
			//���� Ŭ��
			Input->BindAction(LeftClickAction, ETriggerEvent::Started, this, &USMCharacterMoveComponent::Doubleclick);
			// ��Ŭ�� �׼� ���ε�
			Input->BindAction(RightClickAction, ETriggerEvent::Started, this, &USMCharacterMoveComponent::OnRightClick);
			Input->BindAction(RightClickAction, ETriggerEvent::Completed, this, &USMCharacterMoveComponent::OnRightClick);
			
			// �̵� �׼� ���ε�
			Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &USMCharacterMoveComponent::QuaterMove);

			//�巡�� �׼�
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

void USMCharacterMoveComponent::QuaterMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (bIsLeftClicking)
	{

		PlayerController->bShowMouseCursor = false;

		UCameraComponent* Camera = OwningActor->FindComponentByClass<UCameraComponent>();
		ensure(Camera);
		// ��Ŭ�� ���¿����� �̵� ó��
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

		PlayerController->bShowMouseCursor = false;

		// ��Ŭ�� ���¿����� ȭ�� ȸ�� ó��

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

//��Ŭ��
void USMCharacterMoveComponent::OnLeftClick(const FInputActionValue& Value)
{
	bIsLeftClicking = Value.Get<bool>();


	if (PlayerController)
	{
		FInputModeGameOnly InputMode;
		PlayerController->SetInputMode(InputMode);

		if(!bIsLeftClicking)
		{
			PlayerController->bShowMouseCursor = true;
			PlayerController->bEnableClickEvents = true;
			PlayerController->bEnableMouseOverEvents = true;

			PlayerController->SetMouseLocation(MouseLocation.X, MouseLocation.Y);
		}
		else
		{
			PlayerController->GetMousePosition(MouseLocation.X, MouseLocation.Y);

		}
	}



}
//��Ŭ��
void USMCharacterMoveComponent::OnRightClick(const FInputActionValue& Value)
{
	bIsRightClicking = Value.Get<bool>();

	if (PlayerController)
	{

		FInputModeGameOnly InputMode;
		PlayerController->SetInputMode(InputMode);


		if (!bIsRightClicking)
		{
			PlayerController->bShowMouseCursor = true;
			PlayerController->bEnableClickEvents = true;
			PlayerController->bEnableMouseOverEvents = true;

			PlayerController->SetMouseLocation(MouseLocation.X, MouseLocation.Y);
		}
		else
		{
			PlayerController->GetMousePosition(MouseLocation.X, MouseLocation.Y);
		}
	}
}
//�ٷ� ���Ʒ�
void USMCharacterMoveComponent::UpDown(const FInputActionValue& Value)
{
	float Height = FMath::Clamp(OwningActor->GetActorLocation().Z * 0.5, 0, 10000);
	float Power = Value.Get<float>();
	FVector CurrentLocation = OwningActor->GetActorLocation();
	FVector NewLocation = CurrentLocation + FVector(0.0f, 0.0f, Power * Height); // �̵� �ӵ��� �����մϴ�.

	OwningActor->SetActorLocation(NewLocation, true); // true�� �浹�� ����Ͽ� ��ġ�� �����մϴ�.
}

void USMCharacterMoveComponent::Doubleclick(const FInputActionValue& Value)
{
	if (bIsClicking)
	{
		// ���� Ŭ�� ó��
		OnDoubleClick();

		// Ÿ�̸Ӹ� ����ϰ�, Ŭ�� ���� �ʱ�ȭ
		GetWorld()->GetTimerManager().ClearTimer(ClickTimerHandle);
		bIsClicking = false;
	}
	else
	{
		// ù ��° Ŭ�� ó��
		bIsClicking = true;

		// ���� �ð� �Ŀ� ���� Ŭ���� ó���ϵ��� Ÿ�̸� ����
		GetWorld()->GetTimerManager().SetTimer(ClickTimerHandle, this, &USMCharacterMoveComponent::OnSingleClick, DoubleClickMaxTime, false);
	}
}

void USMCharacterMoveComponent::OnDoubleClick() //���⿡ �Լ� ������ �˴ϴ�
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("DoubleClick")));
}

void USMCharacterMoveComponent::OnSingleClick()
{

	bIsClicking = false;
}



