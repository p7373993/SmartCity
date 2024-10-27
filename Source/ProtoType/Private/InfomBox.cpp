#include "InfomBox.h"
#include "Components/TextBlock.h"

void UInfomBox::DisplayInformWidget(const FString& BuildingName, const FString& BuildingAddress)
{
    UWorld* World = GetWorld();

    // InformBoxChildClass가 null일 경우 동적으로 블루프린트를 로드
    if (!InformBoxChildClass)
    {
        FStringClassReference WidgetClassRef(TEXT("/Game/Kichan/WB_Inform.WB_Inform_C"));  // 실제 경로로 수정
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

    // 자식 블루프린트 위젯을 생성하고 데이터 설정 및 뷰포트에 추가
    UInfomBox* ChildWidget = CreateChildInformWidget(BuildingName, BuildingAddress);
    if (ChildWidget)
    {
        // 뷰포트에 추가
        ChildWidget->AddToViewport();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to create child widget!"));
    }
}

void UInfomBox::DisplayInformWidget(const int Bulidingindex, const int tag)
{
    // InformBoxChildClass가 null일 경우 동적으로 블루프린트를 로드
    if (!InformBoxChildClass)
    {
        FStringClassReference WidgetClassRef(TEXT("/Game/Kichan/WB_Inform.WB_Inform_C"));  // 실제 경로로 수정
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

    // 여기에 index 기준으로 받아온 건물 이름이랑 주소 넣으면 됩니다~
    FString BuildingName = "buliding name";
    FString BuildingAddress = "buliding address";

    // 자식 블루프린트 위젯을 생성하고 데이터 설정 및 뷰포트에 추가
    UInfomBox* ChildWidget = CreateChildInformWidget(BuildingName, BuildingAddress);
    if (ChildWidget)
    {
        // 뷰포트에 추가
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

    // 자식 블루프린트 위젯 생성
    UInfomBox* ChildWidget = CreateWidget<UInfomBox>(World, InformBoxChildClass);

    if (ChildWidget)
    {
        // 자식 위젯에 BuildingName과 BuildingAddress 설정
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
    // TextBlock을 찾고 이름과 주소를 설정
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
