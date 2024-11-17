// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/DecalAct.h"

// Sets default values
ADecalAct::ADecalAct()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADecalAct::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADecalAct::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

