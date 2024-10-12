// Fill out your copyright notice in the Description page of Project Settings.


#include "InfomBox.h"
#include "Components/TextBlock.h"

UInfomBox* UInfomBox::CreateAndDisplay(UObject* WorldContextObject, TSubclassOf<UInfomBox> InformClass, FString BuildingName, FString BuildingAddress)
{
    // 위젯을 생성합니다.
    UInfomBox* InformWidget = CreateWidget<UInfomBox>(WorldContextObject, InformClass);

    if (InformWidget)
    {
        // 데이터를 설정하고 뷰포트에 추가합니다.
        InformWidget->DisplayData(BuildingName, BuildingAddress);
        InformWidget->AddToViewport();
    }

    return InformWidget;
}

void UInfomBox::DisplayData(FString BuildingName, FString BuildingAddress)
{
    // TextBlock에 데이터 설정
    if (BuildingNameText && BuildingAddressText)
    {
        BuildingNameText->SetText(FText::FromString(BuildingName));
        BuildingAddressText->SetText(FText::FromString(BuildingAddress));
    }
}
