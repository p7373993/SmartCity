#include "InfomBox.h"
#include "Components/TextBlock.h"

void UInfomBox::DisplayInformWidget(const FString& BuildingName, const FString& BuildingAddress)
{
    UWorld* World = GetWorld();

    // InformBoxChildClass�� null�� ��� �������� �������Ʈ�� �ε�
    if (!InformBoxChildClass)
    {
        FStringClassReference WidgetClassRef(TEXT("/Game/Kichan/WB_Inform.WB_Inform_C"));  // ���� ��η� ����
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

    // �ڽ� �������Ʈ ������ �����ϰ� ������ ���� �� ����Ʈ�� �߰�
    UInfomBox* ChildWidget = CreateChildInformWidget(BuildingName, BuildingAddress);
    if (ChildWidget)
    {
        // ����Ʈ�� �߰�
        ChildWidget->AddToViewport();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to create child widget!"));
    }
}

void UInfomBox::DisplayInformWidget(const int Bulidingindex, const int tag)
{
    // InformBoxChildClass�� null�� ��� �������� �������Ʈ�� �ε�
    if (!InformBoxChildClass)
    {
        FStringClassReference WidgetClassRef(TEXT("/Game/Kichan/WB_Inform.WB_Inform_C"));  // ���� ��η� ����
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

    // ���⿡ index �������� �޾ƿ� �ǹ� �̸��̶� �ּ� ������ �˴ϴ�~
    FString BuildingName = "buliding name";
    FString BuildingAddress = "buliding address";

    // �ڽ� �������Ʈ ������ �����ϰ� ������ ���� �� ����Ʈ�� �߰�
    UInfomBox* ChildWidget = CreateChildInformWidget(BuildingName, BuildingAddress);
    if (ChildWidget)
    {
        // ����Ʈ�� �߰�
        ChildWidget->AddToViewport();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to create child widget!"));
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

    // �ڽ� �������Ʈ ���� ����
    UInfomBox* ChildWidget = CreateWidget<UInfomBox>(World, InformBoxChildClass);

    if (ChildWidget)
    {
        // �ڽ� ������ BuildingName�� BuildingAddress ����
        ChildWidget->SetBuildingInfo(BuildingName, BuildingAddress);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create InformBox child widget!"));
    }

    return ChildWidget;
}

void UInfomBox::SetBuildingInfo(const FString& BuildingName, const FString& BuildingAddress)
{
    // TextBlock�� ã�� �̸��� �ּҸ� ����
    UTextBlock* NameTextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("BuildingName")));
    UTextBlock* AddressTextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("BuildingAddress")));

    if (NameTextBlock && AddressTextBlock)
    {
        NameTextBlock->SetText(FText::FromString(BuildingName));
        AddressTextBlock->SetText(FText::FromString(BuildingAddress));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("TextBlocks not found!"));
    }
}
