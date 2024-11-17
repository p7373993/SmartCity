// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/DecalAct.h"
/**
 * 
 */
class PROTOTYPE_API DecalActSpawnManager
{
public:

	static DecalActSpawnManager& GetInstance()
	{
		static DecalActSpawnManager instance;
		return instance;
	}

	ADecalAct* SpawnDecalActor(UWorld* wolrd, FVector location);
	
	void HideAllActor();
	void VisibleAllActor();
	void ClearAllActor();

	ADecalAct* FindActor(int Index);
	void DeleteActor(int Index);

	TArray<ADecalAct*> DecalActSpawnedInstance;
private:
	DecalActSpawnManager(const DecalActSpawnManager&) = delete;
	DecalActSpawnManager& operator=(const DecalActSpawnManager&) = delete;

	DecalActSpawnManager();
};