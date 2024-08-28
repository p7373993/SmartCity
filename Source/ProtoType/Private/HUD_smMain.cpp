// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD_smMain.h"
#include "UW_smMain.h"
#include "GameFramework/PlayerController.h"

AHUD_smMain::AHUD_smMain()
{
    static ConstructorHelpers::FClassFinder<UUW_smMain> UI_Main_C(TEXT("/Game/Kichan/WB_Main.WB_Main_C"));
    MyWidgetClass = UI_Main_C.Class;
}

void AHUD_smMain::BeginPlay()
{
    Super::BeginPlay();

    if (MyWidgetClass)
    {

        APlayerController* PlayerController = GetOwningPlayerController();

        if (PlayerController)
        {
            PlayerController->bShowMouseCursor = true;

            MyWidget = CreateWidget<UUW_smMain>(PlayerController, MyWidgetClass);
            if (MyWidget)
            {
                MyWidget->AddToViewport();
            }
        }
    }
}
