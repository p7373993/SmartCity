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
#include "InfomBox.h"
#include "ProtoType/private/ClientModule/TCPModule.h"
#include <String>
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

	PrimaryComponentTick.bCanEverTick = true;
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

void USMCharacterMoveComponent::StartMovingToLocation(const FVector& targetLocation)
{
	this->TargetLocation = targetLocation;
	bIsMovingToTarget = true; // �̵��� �����ϵ��� ����
}

void USMCharacterMoveComponent::MoveToLocation()
{
	FVector CurrentLocation = OwningActor->GetActorLocation();
	float DistanceToTarget = FVector::Dist(CurrentLocation, TargetLocation);

	// ��ǥ ��ġ���� �Ÿ��� ���� �ӵ��� �����մϴ� (�ָ� �� ������ �̵�)
	float Speed = FMath::Clamp(DistanceToTarget * 0.1f, 1000000.0f, 100000000.0f);
	UE_LOG(LogTemp, Warning, TEXT("My Speed is: %f"), Speed);
	UE_LOG(LogTemp, Warning, TEXT("My DistanceToTarget is: %f"), DistanceToTarget);
	FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();
	FVector NewLocation = CurrentLocation + Direction * Speed * GetWorld()->DeltaTimeSeconds;

	if (DistanceToTarget <= Speed * GetWorld()->DeltaTimeSeconds)
	{
		NewLocation = TargetLocation;
		bIsMovingToTarget = false; // ��ǥ ��ġ�� �����ϸ� �̵��� ����ϴ�.

		if (PlayerController)
		{
			FRotator NewRotation = PlayerController->GetControlRotation();
			NewRotation.Yaw = 240.0f;
			PlayerController->SetControlRotation(NewRotation);
		}
	}

	OwningActor->SetActorLocation(NewLocation);
}

void USMCharacterMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsMovingToTarget)
	{
		MoveToLocation();
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

					//UInfomBox* InformBox = CreateWidget<UInfomBox>(GetWorld(), UInfomBox::StaticClass());
					UInfomBox* InformBox = UInfomBox::GetInstance(GetWorld());
					if (InformBox)
					{
						int Type = 2;

						bool isNumeric = true;
						for (char ch : Tag.ToString())
						{
							if (!isdigit(ch))
							{
								isNumeric = false;
								break;
							}
						}

						int number = 0;
						if (isNumeric)
						{
							std::string Temp = std::string(TCHAR_TO_UTF8(*Tag.ToString()));
							int TagIntValue = std::stoi(Temp);
							number = std::stoi(Temp);
							float TempEL[20] = { Type, TagIntValue, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
							TCPModule& TCPModuleA = TCPModule::GetInstance();
							TextStruct TempText = TCPModuleA.GetBuildingAddressAndName(TempEL, 2);

							InformBox->DisplayInformWidget(TempText.BuildingName, TempText.BuildingAddress);
						}
					}
				}
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No Tags Found"));
				//UInfomBox* InformBox = UInfomBox::GetInstance(GetWorld());
				//if (InformBox)
				//{
				//	InformBox->DisplayInformWidget("No Data", "No Data");
				//}
			}
		}
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
	GetActorTag();


	if (PlayerController)
	{

		if(!bIsLeftClicking)
		{
			FInputModeGameAndUI InputMode;
			PlayerController->SetInputMode(InputMode);

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


		if (!bIsRightClicking)
		{

			FInputModeGameAndUI InputMode;
			PlayerController->SetInputMode(InputMode);

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



