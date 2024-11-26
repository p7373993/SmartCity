// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HUD_smMain.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE_API AHUD_smMain : public AHUD
{
	GENERATED_BODY()

public:
    AHUD_smMain();

    //static UUW_smMain* GetInstance(UObject* WorldContextObject);

    APlayerController* PlayerController;

    virtual void BeginPlay() override;

    virtual void PostInitializeComponents() override;

    class UUW_smMain* getHudWidget();

protected:
    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<class UUW_smMain> MyWidgetClass;

    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<class USearchBox> MainSearchBarClass;

    //UPROPERTY(EditAnywhere, Category = "UI")
    //TSubclassOf<class UNameBox> NameTextBoxWidgetClass;

    void HandleButtonClicked();

private:
    //static UUW_smMain* MyWidgetInstance;
    UUW_smMain* MyWidget;
    USearchBox* MainSearchBar;
    //UNameBox* MainNameBox;
};
