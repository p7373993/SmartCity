// Fill out your copyright notice in the Description page of Project Settings.


#include "Module/OpenWorldComponent.h"

// Sets default values for this component's properties
UOpenWorldComponent::UOpenWorldComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenWorldComponent::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World)
	{
		// 델리게이트에 바인딩
		FWorldDelegates::LevelAddedToWorld.AddUObject(this, &UOpenWorldComponent::OnLevelLoaded);
	}
}


// Called every frame
void UOpenWorldComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UOpenWorldComponent::OnLevelLoaded(ULevel* InLevel, UWorld* InWorld)
{

	LevelLoad.ExecuteIfBound();

}

