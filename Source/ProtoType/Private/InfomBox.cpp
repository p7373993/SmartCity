#include "InfomBox.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

UInfomBox* UInfomBox::Instance = nullptr;

UInfomBox* UInfomBox::GetInstance(UObject* WorldContextObject)
{
    if (!Instance)
    {
        Instance = CreateWidget<UInfomBox>(WorldContextObject->GetWorld(), UInfomBox::StaticClass());
    }
    else
    {
        Instance->RemoveFromViewport();
    }

    return Instance; 
}

void UInfomBox::NativeConstruct()
{
    if (!RemoveBtn)
    {
        RemoveBtn = Cast<UButton>(GetWidgetFromName(TEXT("remove_btn")));
    }
    if (RemoveBtn)
    {
        RemoveBtn->OnClicked.AddDynamic(this, &UInfomBox::RemoveBtnClicked);
    }
}

void UInfomBox::DisplayInformWidget(const FString& BuildingName, const FString& BuildingAddress)
{
    UWorld* World = GetWorld();

    if (!InformBoxChildClass)
    {
        FStringClassReference WidgetClassRef(TEXT("/Game/Kichan/WB_Inform.WB_Inform_C"));  // ½ÇÁ¦ °æ·Î·Î ¼öÁ¤
        UClass* WidgetClass = WidgetClassRef.TryLoadClass<UUserWidget>();

        if (WidgetClass)
        {
            InformBoxChildClass = WidgetClass;
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to load InformBoxChildClass!"));
            return;
        }
    }

    //UInfomBox* ChildWidget = CreateChildInformWidget(BuildingName, BuildingAddress);
    //if (Instance->cChildWidget)
    //{
    //    cChildWidget->RemoveFromParent();
    //}
    cChildWidget = CreateChildInformWidget(BuildingName, BuildingAddress);
    cChildWidget->AddToViewport();

}

//void UInfomBox::DisplayInformWidget(const FString& BuildingName, const FString& BuildingAddress)
//{
//    if (Instance)
//    {
//        Instance->RemoveFromParent();
//    }
//
//    Instance = CreateWidget<UInfomBox>(GetWorld(), UInfomBox::StaticClass());
//    if (Instance)
//    {
//        UTextBlock* NameTextBlock = Cast<UTextBlock>(Instance->GetWidgetFromName(TEXT("BuildingName")));
//        UTextBlock* AddressTextBlock = Cast<UTextBlock>(Instance->GetWidgetFromName(TEXT("BuildingAddress")));
//
//        if (NameTextBlock && AddressTextBlock)
//        {
//            NameTextBlock->SetText(FText::FromString(BuildingName));
//            AddressTextBlock->SetText(FText::FromString(BuildingAddress));
//        }
//        Instance->AddToViewport();
//    }
//}

void UInfomBox::DisplayInformWidget(const int Bulidingindex, const int tag)
{
    if (!InformBoxChildClass)
    {
        FStringClassReference WidgetClassRef(TEXT("/Game/Kichan/WB_Inform.WB_Inform_C"));  
        UClass* WidgetClass = WidgetClassRef.TryLoadClass<UUserWidget>();

        if (WidgetClass)
        {
            InformBoxChildClass = WidgetClass;
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to load InformBoxChildClass!"));
            return;
        }
    }

    FString BuildingName = "buliding name";
    FString BuildingAddress = "buliding address";

    UInfomBox* ChildWidget = CreateChildInformWidget(BuildingName, BuildingAddress);
    if (ChildWidget)
    {
        ChildWidget->AddToViewport();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to create child widget!"));
    }
}



void UInfomBox::RemoveInstance()
{
    if (Instance)
    {
        Instance->RemoveFromParent();
        Instance = nullptr;
    }
}

UInfomBox* UInfomBox::CreateChildInformWidget(const FString& BuildingName, const FString& BuildingAddress)
{
    UWorld* World = GetWorld();
    if (!World || !InformBoxChildClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid World or InformBoxChildClass"));
        return nullptr;
    }

    UInfomBox* ChildWidget = CreateWidget<UInfomBox>(World, InformBoxChildClass);

    //if (ChildWidget)
    //{
    //    ChildWidget->SetBuildingInfo(BuildingName, BuildingAddress);
    //}
    //else
    //{
    //    UE_LOG(LogTemp, Error, TEXT("Failed to create InformBox child widget!"));
    //}

    //return ChildWidget;

    if (Instance)
    {
        Instance = ChildWidget;
        Instance->SetBuildingInfo(BuildingName, BuildingAddress);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create InformBox child widget!"));
    }

    return Instance;
}

void UInfomBox::SetBuildingInfo(const FString& BuildingName, const FString& BuildingAddress)
{
    /*UTextBlock* NameTextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("BuildingName")));
    UTextBlock* AddressTextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("BuildingAddress")));

    if (NameTextBlock && AddressTextBlock)
    {
        NameTextBlock->SetText(FText::FromString(BuildingName));
        AddressTextBlock->SetText(FText::FromString(BuildingAddress));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("TextBlocks not found!"));
    }*/
    UTextBlock* NameTextBlock = Cast<UTextBlock>(Instance->GetWidgetFromName(TEXT("BuildingName")));
    UTextBlock* AddressTextBlock = Cast<UTextBlock>(Instance->GetWidgetFromName(TEXT("BuildingAddress")));
    
    if (NameTextBlock && AddressTextBlock)
    {
        NameTextBlock->SetText(FText::FromString(BuildingName));
        AddressTextBlock->SetText(FText::FromString(BuildingAddress));
    }
}

void UInfomBox::RemoveBtnClicked()
{
    RemoveFromParent();
    //RemoveInstance();
}
