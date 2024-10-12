// Fill out your copyright notice in the Description page of Project Settings.


#include "InfomBox.h"
#include "Components/TextBlock.h"

UInfomBox* UInfomBox::CreateAndDisplay(UObject* WorldContextObject, TSubclassOf<UInfomBox> InformClass, FString BuildingName, FString BuildingAddress)
{
    // ������ �����մϴ�.
    UInfomBox* InformWidget = CreateWidget<UInfomBox>(WorldContextObject, InformClass);

    if (InformWidget)
    {
        // �����͸� �����ϰ� ����Ʈ�� �߰��մϴ�.
        InformWidget->DisplayData(BuildingName, BuildingAddress);
        InformWidget->AddToViewport();
    }

    return InformWidget;
}

void UInfomBox::DisplayData(FString BuildingName, FString BuildingAddress)
{
    // TextBlock�� ������ ����
    if (BuildingNameText && BuildingAddressText)
    {
        BuildingNameText->SetText(FText::FromString(BuildingName));
        BuildingAddressText->SetText(FText::FromString(BuildingAddress));
    }
}
