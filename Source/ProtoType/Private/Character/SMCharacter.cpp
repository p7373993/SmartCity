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
		MovementComponent->GravityScale = 0.0f; // �߷� ��Ȱ��ȭ
		MovementComponent->AirControl = 1.0f;  // ���߿��� �̵� ���� Ȱ��ȭ
		MovementComponent->JumpZVelocity = 0.0f; // ���� �ӵ� ��Ȱ��ȭ
	}

	DefaultMove = CreateDefaultSubobject<USMCharacterMoveComponent>(TEXT("DefaultMove"));//���콺 ������ ���ȭ
	RayCast = CreateDefaultSubobject<USMPointComponent>(TEXT("RayCast"));//���콺 ������ ���ȭ
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

