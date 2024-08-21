// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawn2.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
// Sets default values
AMyPawn2::AMyPawn2()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyPawn2::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyPawn2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyPawn2::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
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
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyPawn2::QuaterMove);
	}
}

void AMyPawn2::QuaterMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	float MovmentVectorSize = 1.0f;
	float MovementVectorSizeSquared = MovementVector.SquaredLength();



	FVector MoveDirection = FVector{ MovementVector.X,MovementVector.Y,0.0f };

	AddMovementInput(MoveDirection, MovementVectorSizeSquared);

	FString DebugMessage = FString::Printf(TEXT("(%f:%f)"), MovementVector.X, MovementVector.Y);
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, DebugMessage);
}

