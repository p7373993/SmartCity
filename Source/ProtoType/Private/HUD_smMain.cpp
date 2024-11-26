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

    /*static ConstructorHelpers::FClassFinder<USearchBox> UI_SearchBar_C(TEXT("/Game/Kichan/W_SearchBox.W_SearchBox_C"));
    MainSearchBarClass = UI_SearchBar_C.Class;*/

    static ConstructorHelpers::FClassFinder<USearchBox> UI_SearchBar_C(TEXT("/Game/Kichan/WB_SearchBox.WB_SearchBox_C"));
    MainSearchBarClass = UI_SearchBar_C.Class;

    //static ConstructorHelpers::FClassFinder<UNameBox> NameTextBoxBPClass(TEXT("/Game/Kichan/W_NameBox.W_NameBox_C")); // �������Ʈ ���
    //if (NameTextBoxBPClass.Succeeded())
    //{
    //    NameTextBoxWidgetClass = NameTextBoxBPClass.Class;
    //}
}

//UUW_smMain* AHUD_smMain::MyWidgetInstance = nullptr;
//UUW_smMain* AHUD_smMain::GetInstance(UObject* WorldContextObject)
//{
//    if (!MyWidgetInstance)
//    {
//        MyWidgetInstance = CreateWidget<UUW_smMain>(WorldContextObject->GetWorld(), UUW_smMain::StaticClass());
//    }
//    return MyWidgetInstance;
//}

void AHUD_smMain::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    if (MyWidgetClass)
    {
        MyWidget = CreateWidget<UUW_smMain>(GetWorld(), MyWidgetClass);
        if (MyWidget)
        {
            MyWidget->AddToViewport();
        }
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

            //MyWidget = CreateWidget<UUW_smMain>(PlayerController, MyWidgetClass);
            //if (MyWidget)
            //{
            //    MyWidget->AddToViewport();
            //    //MyWidget->OnButtonStateChanged.AddDynamic(this, &AHUD_smMain::HandleButtonClicked);
            //}
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

UUW_smMain* AHUD_smMain::getHudWidget()
{
    return MyWidget;
}

void AHUD_smMain::HandleButtonClicked()
{

}
