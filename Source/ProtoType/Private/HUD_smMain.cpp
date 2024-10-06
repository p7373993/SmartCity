// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD_smMain.h"
#include "UW_smMain.h"
#include "SearchBox.h"
#include "NameBox.h"
#include "GameFramework/PlayerController.h"

AHUD_smMain::AHUD_smMain()
{
    static ConstructorHelpers::FClassFinder<UUW_smMain> UI_Main_C(TEXT("/Game/Kichan/WB_Main.WB_Main_C"));
    MyWidgetClass = UI_Main_C.Class;

    static ConstructorHelpers::FClassFinder<USearchBox> UI_SearchBar_C(TEXT("/Game/Kichan/W_SearchBox.W_SearchBox_C"));
    MainSearchBarClass = UI_SearchBar_C.Class;

    static ConstructorHelpers::FClassFinder<UNameBox> NameTextBoxBPClass(TEXT("/Game/Kichan/W_NameBox.W_NameBox_C")); // 블루프린트 경로
    if (NameTextBoxBPClass.Succeeded())
    {
        NameTextBoxWidgetClass = NameTextBoxBPClass.Class;
    }
}

void AHUD_smMain::BeginPlay()
{
    Super::BeginPlay();
    PlayerController = GetOwningPlayerController();
    if (MyWidgetClass)
    {
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

    if (MainSearchBarClass)
    {
        if (PlayerController)
        {
            MainSearchBar = CreateWidget<USearchBox>(PlayerController, MainSearchBarClass);
            if (MainSearchBar)
            {
                MainSearchBar->AddToViewport();
            }
        }
    }
}
