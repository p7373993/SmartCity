// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"

// Sets default values
AMyPawn::AMyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MovementSpeed = 600.0f;

	TurnRate = 45.0f;
	LookUpRate = 45.0f;

	//USpringArmComponent* SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	//SpringArm->SetupAttachment(RootComponent);
	//SpringArm->TargetArmLength = 300.0f;
	//SpringArm->bUsePawnControlRotation = true;

	UCameraComponent* Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camer"));
	Camera->SetupAttachment(RootComponent);
	Camera->bUsePawnControlRotation = true;

	static ConstructorHelpers::FObjectFinder<UInputAction> QUaterMoveRef = TEXT("/Script/EnhancedInput.InputAction'/Game/Sungwoo/Input/IA_Move.IA_Move'");
	check(QUaterMoveRef.Object);
	if (QUaterMoveRef.Object)QuaterMoveAction = QUaterMoveRef.Object;

}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(QuaterMoveAction, ETriggerEvent::Triggered, this, &AMyPawn::QuaterMove);
	//PlayerInputComponent->BindAxis("MoveForward", this, &AMyPawn::MoveForward);
	//PlayerInputComponent->BindAxis("MoveRight", this, &AMyPawn::MoveRight);

	//PlayerInputComponent->BindAxis("Turn", this, &AMyPawn::Turn);
	//PlayerInputComponent->BindAxis("LookUp", this, &AMyPawn::LookUp);
}

void AMyPawn::QuaterMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	float MovmentVectorSize = 1.0f;
	float MovementVectorSizeSquared = MovementVector.SquaredLength();
	if (MovementVectorSizeSquared > 1.0f) {
		MovementVectorSizeSquared = 1.0f;
		MovementVector.Normalize();
	}


		FVector MoveDirection = FVector{ MovementVector.X,MovementVector.Y,0.0f };

		AddMovementInput(MoveDirection, MovementVectorSizeSquared);
		//GetController()->SetControlRotation(FRotationMatrix::MakeFromX(MoveDirection).Rotator());
}

//void AMyPawn::MoveForward(float Value)
//{
//	AddMovementInput(GetActorForwardVector(), Value * MovementSpeed);
//}
//
//void AMyPawn::MoveRight(float Value)
//{
//	AddMovementInput(GetActorRightVector(), Value * MovementSpeed);
//}
//
//void AMyPawn::Turn(float Rate)
//{
//	AddControllerYawInput(Rate * TurnRate * GetWorld()->GetDeltaSeconds());
//}
//
//void AMyPawn::LookUp(float Rate)
//{
//	AddControllerPitchInput(Rate * LookUpRate * GetWorld()->GetDeltaSeconds());
//}

