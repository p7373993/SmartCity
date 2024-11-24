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
        //DecalInstance
    }
    DecalActSpawnedInstance.Add(DecalInstance);
	return DecalInstance;
}

void DecalActSpawnManager::HideAllActor()
{
    for (ADecalAct* Temp : DecalActSpawnedInstance) {
        if (Temp) {
            Temp->IsHidden = true;
            Temp->SetActorHiddenInGame(true);
        }
    }
}

void DecalActSpawnManager::VisibleAllActor()
{
    for (ADecalAct* Temp : DecalActSpawnedInstance) {
        if (Temp) {
            Temp->IsHidden = false;
            Temp->SetActorHiddenInGame(false);
        }
    }
}

void DecalActSpawnManager::ClearAllActor()
{
    for (ADecalAct* Temp : DecalActSpawnedInstance) {
        if (Temp && !Temp->IsPendingKill()) {
            Temp->Destroy();
        }
    }
    DecalActSpawnedInstance.Empty();
}

ADecalAct* DecalActSpawnManager::FindActor(int Index)
{
    for (ADecalAct* Temp : DecalActSpawnedInstance) {
        if (Temp) {
            if (Temp->Index == Index)
            {
                return Temp;
            }
        }
    }
    return nullptr;
}

void DecalActSpawnManager::DeleteActor(int Index)
{
    for (int i = DecalActSpawnedInstance.Num() - 1; i >= 0; --i) {
        ADecalAct* Temp = DecalActSpawnedInstance[i];
        if (Temp && Temp->Index == Index) {
            Temp->Destroy();
            DecalActSpawnedInstance.RemoveAt(i);
        }
    }
}
