// Fill out your copyright notice in the Description page of Project Settings.

#include "Manager/DecalActSpawnManager.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

DecalActSpawnManager::DecalActSpawnManager()
{
}

ADecalAct* DecalActSpawnManager::SpawnDecalActor(UWorld* World, FVector Location, int Index)
{

    ADecalAct* DecalInstance;
    UBlueprint* ObjectToSpawn = Cast<UBlueprint>(StaticLoadObject(UBlueprint::StaticClass(), NULL, TEXT("/Game/Blueprints/BP_Decal.BP_Decal")));
    if (ObjectToSpawn)
    {
        FRotator SpawnRotation = FRotator(0, 0, 0);
        DecalInstance = World->SpawnActor<ADecalAct>(ObjectToSpawn->GeneratedClass, Location, SpawnRotation);
    }
    DecalActSpawnedInstance.Add(DecalInstance);
	return DecalInstance;
}

