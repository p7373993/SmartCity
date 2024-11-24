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

	/** 데칼 엑터를 UI 클릭으로 스폰할때 사용 */
	ADecalAct* SpawnDecalActor(UWorld* wolrd, FVector location, int Index);
	
	/** UI 2번째 버튼 비활성화로 액터를 비표기할때 사용 */
	void HideAllActor();

	/** UI 2번째 버튼 활성화로 액터를 표기할때 사용 */
	void VisibleAllActor();

	/** 모든 액터 삭제 버튼을 클릭했을때 사용 */
	void ClearAllActor();


	/** 데칼 액터의 속성을 찾고싶으면 사용 */
	ADecalAct* FindActor(int Index);
	

	/** 데칼 액터를 인덱스 번호로 지울때 사용 */
	void DeleteActor(int Index);

	TArray<ADecalAct*> DecalActSpawnedInstance;
private:
	DecalActSpawnManager(const DecalActSpawnManager&) = delete;
	DecalActSpawnManager& operator=(const DecalActSpawnManager&) = delete;

	DecalActSpawnManager();
};