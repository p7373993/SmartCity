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

    return Instance; // ÀÌ¹Ì »ý¼ºµÈ ÀÎ½ºÅÏ½º ¹ÝÈ¯
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

    // InformBoxChildClass°¡ nullÀÏ °æ¿ì µ¿ÀûÀ¸·Î ºí·çÇÁ¸°Æ®¸¦ ·Îµå
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

    // ÀÚ½Ä ºí·çÇÁ¸°Æ® À§Á¬À» »ý¼ºÇÏ°í µ¥ÀÌÅÍ ¼³Á¤ ¹× ºäÆ÷Æ®¿¡ Ãß°¡
    //UInfomBox* ChildWidget = CreateChildInformWidget(BuildingName, BuildingAddress);
    
    if (Instance)
    {
        // ÀÌ¹Ì ÀÖÀ» °æ¿ì Áö¿î ÈÄ
        // Ãß°¡
        Instance->RemoveFromParent();
    }

    cChildWidget = CreateChildInformWidget(BuildingName, BuildingAddress);
    cChildWidget->AddToViewport();
}

//ºôµù ÀÎµ¦½º, Å×±× ±â¹ÝÀ¸·Î Á¤º¸Ã¢ »ý¼º
void UInfomBox::DisplayInformWidget(const int Bulidingindex, const int tag)
{
    // InformBoxChildClass°¡ nullÀÏ °æ¿ì µ¿ÀûÀ¸·Î ºí·çÇÁ¸°Æ®¸¦ ·Îµå
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

    // ¿©±â¿¡ index ±âÁØÀ¸·Î ¹Þ¾Æ¿Â °Ç¹° ÀÌ¸§ÀÌ¶û ÁÖ¼Ò ³ÖÀ¸¸é µË´Ï´Ù~
    FString BuildingName = "buliding name";
    FString BuildingAddress = "buliding address";

    // ÀÚ½Ä ºí·çÇÁ¸°Æ® À§Á¬À» »ý¼ºÇÏ°í µ¥ÀÌÅÍ ¼³Á¤ ¹× ºäÆ÷Æ®¿¡ Ãß°¡
    UInfomBox* ChildWidget = CreateChildInformWidget(BuildingName, BuildingAddress);
    if (ChildWidget)
    {
        // ºäÆ÷Æ®¿¡ Ãß°¡
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

    // ÀÚ½Ä ºí·çÇÁ¸°Æ® À§Á¬ »ý¼º
    UInfomBox* ChildWidget = CreateWidget<UInfomBox>(World, InformBoxChildClass);

    if (ChildWidget)
    {
        // ÀÚ½Ä À§Á¬¿¡ BuildingName°ú BuildingAddress ¼³Á¤
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
    // TextBlockÀ» Ã£°í ÀÌ¸§°ú ÁÖ¼Ò¸¦ ¼³Á¤
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

void UInfomBox::RemoveBtnClicked()
{
    RemoveFromParent();
}
