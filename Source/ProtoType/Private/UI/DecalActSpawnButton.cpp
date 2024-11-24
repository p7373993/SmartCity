// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DecalActSpawnButton.h"
#include "Components/Button.h"
#include "Components/Widget.h"

void UDecalActSpawnButton::NativeConstruct()
{
    Super::NativeConstruct();

    if (Button1)
    {
        Button1->OnClicked.AddDynamic(this, &UDecalActSpawnButton::OnButtonClicked);
    }

    if (Button2)
    {
        Button2->OnClicked.AddDynamic(this, &UDecalActSpawnButton::offButtonClicked);
    }
}

void UDecalActSpawnButton::InitializeWithActor(AUIActor* TOwnUIAct)
{
    OwnUIAct = TOwnUIAct;
}

void UDecalActSpawnButton::OnButtonClicked()
{
    SpawnAct();
    if (Button1)
    {
        Button1->SetVisibility(ESlateVisibility::Hidden);
    }
    if (Button2)
    {
        Button2->SetVisibility(ESlateVisibility::Visible);
    }
}

void UDecalActSpawnButton::offButtonClicked()
{
    DeleteAct();
    if (Button1)
    {
        Button1->SetVisibility(ESlateVisibility::Visible);
    }
    if (Button2)
    {
        Button2->SetVisibility(ESlateVisibility::Hidden);
    }
}

void UDecalActSpawnButton::SpawnAct()
{
    OwnUIAct->SpawnAct();
}

void UDecalActSpawnButton::DeleteAct()
{
    OwnUIAct->DeleteAct();
}
