// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SMCharacter.h"
#include "Camera/CameraComponent.h"
#include "Character/SMCharacterMoveComponent.h"
#include "Module/SMPointComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ASMCharacter::ASMCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);
	Camera->bUsePawnControlRotation = true;

	if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
	{
		MovementComponent->GravityScale = 0.0f; // 중력 비활성화
		MovementComponent->AirControl = 1.0f;  // 공중에서 이동 조절 활성화
		MovementComponent->JumpZVelocity = 0.0f; // 점프 속도 비활성화
	}

	DefaultMove = CreateDefaultSubobject<USMCharacterMoveComponent>(TEXT("DefaultMove"));//마우스 움직임 모듈화
	RayCast = CreateDefaultSubobject<USMPointComponent>(TEXT("RayCast"));//마우스 움직임 모듈화
}

// Called when the game starts or when spawned
void ASMCharacter::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void ASMCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASMCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

