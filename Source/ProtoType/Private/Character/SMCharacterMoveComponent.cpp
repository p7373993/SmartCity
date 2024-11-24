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
			//더블 클릭
			Input->BindAction(LeftClickAction, ETriggerEvent::Started, this, &USMCharacterMoveComponent::Doubleclick);
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

void USMCharacterMoveComponent::StartMovingToLocation(const FVector& targetLocation)
{
	this->TargetLocation = targetLocation;
	bIsMovingToTarget = true; // 이동을 시작하도록 설정
}

void USMCharacterMoveComponent::MoveToLocation()
{
	FVector CurrentLocation = OwningActor->GetActorLocation();
	float DistanceToTarget = FVector::Dist(CurrentLocation, TargetLocation);

	// 목표 위치까지 거리에 따라 속도를 조절합니다 (멀면 더 빠르게 이동)
	float Speed = FMath::Clamp(DistanceToTarget * 0.1f, 1000000.0f, 100000000.0f);
	UE_LOG(LogTemp, Warning, TEXT("My Speed is: %f"), Speed);
	UE_LOG(LogTemp, Warning, TEXT("My DistanceToTarget is: %f"), DistanceToTarget);
	FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();
	FVector NewLocation = CurrentLocation + Direction * Speed * GetWorld()->DeltaTimeSeconds;

	if (DistanceToTarget <= Speed * GetWorld()->DeltaTimeSeconds)
	{
		NewLocation = TargetLocation;
		bIsMovingToTarget = false; // 목표 위치에 도달하면 이동을 멈춥니다.

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

		PlayerController->bShowMouseCursor = false;

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
//우클릭
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
//휠로 위아래
void USMCharacterMoveComponent::UpDown(const FInputActionValue& Value)
{
	float Height = FMath::Clamp(OwningActor->GetActorLocation().Z * 0.5, 0, 10000);
	float Power = Value.Get<float>();
	FVector CurrentLocation = OwningActor->GetActorLocation();
	FVector NewLocation = CurrentLocation + FVector(0.0f, 0.0f, Power * Height); // 이동 속도를 조정합니다.

	OwningActor->SetActorLocation(NewLocation, true); // true는 충돌을 고려하여 위치를 설정합니다.
}

void USMCharacterMoveComponent::Doubleclick(const FInputActionValue& Value)
{
	if (bIsClicking)
	{
		// 더블 클릭 처리
		OnDoubleClick();

		// 타이머를 취소하고, 클릭 상태 초기화
		GetWorld()->GetTimerManager().ClearTimer(ClickTimerHandle);
		bIsClicking = false;
	}
	else
	{
		// 첫 번째 클릭 처리
		bIsClicking = true;

		// 일정 시간 후에 단일 클릭을 처리하도록 타이머 설정
		GetWorld()->GetTimerManager().SetTimer(ClickTimerHandle, this, &USMCharacterMoveComponent::OnSingleClick, DoubleClickMaxTime, false);
	}
}

void USMCharacterMoveComponent::OnDoubleClick() //여기에 함수 넣으면 됩니다
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("DoubleClick")));
}

void USMCharacterMoveComponent::OnSingleClick()
{

	bIsClicking = false;
}



