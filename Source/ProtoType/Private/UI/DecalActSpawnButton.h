// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "Actor/UIActor.h"
#include "DecalActSpawnButton.generated.h"

/**
 * 
 */
UCLASS()
class UDecalActSpawnButton : public UUserWidget
{
	GENERATED_BODY()
public:
    virtual void NativeConstruct() override;
    void InitializeWithActor(AUIActor* TOwnUIAct);
    AUIActor* OwnUIAct;
protected:
    UPROPERTY(meta = (BindWidget))
    class UButton* Button1;

    UPROPERTY(meta = (BindWidget))
    class UButton* Button2;

    bool IsOn;


    void UIUnVisible();
    void UIVisible();

    UFUNCTION()
    void OnButtonClicked();

    UFUNCTION()
    void offButtonClicked();

    void SpawnAct();
    void DeleteAct();
};
