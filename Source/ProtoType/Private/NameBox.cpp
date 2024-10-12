#include "NameBox.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UNameBox::NativeConstruct()
{
    Super::NativeConstruct();

    if (!NameBtn)
    {
        NameBtn = Cast<UButton>(GetWidgetFromName(TEXT("NameButton")));
    }
    // NameText는 BindWidget을 사용하여 자동으로 바인딩됩니다.
    // BindWidget을 사용하지 않고 동적으로 찾고 싶다면 아래 코드를 사용할 수 있습니다.
    if (!NameText)
    {
        NameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("NameTextBox")));
    }

    // NameText가 정상적으로 바인딩되었는지 확인
    if (NameText)
    {
        // 초기 작업이나 설정이 필요하면 여기에 추가
    }

    if (NameBtn)
    {
        NameBtn->OnClicked.AddDynamic(this, &UNameBox::OnNameBtnClicked);
    }

    // 테스트용 데이터 초기화
    InitializeTestData();
}

void UNameBox::SetName(const FString& InName)
{
    if (NameText)
    {
        NameText->SetText(FText::FromString(InName));
    }
}

void UNameBox::OnNameBtnClicked()
{
    FString NameTextValue = NameText->GetText().ToString();

    // 데이터 검색
    FString* FoundData = FindDataByName(NameTextValue);
    if (FoundData)
    {
        DisplayDataInViewport(NameTextValue, *FoundData);
    }
}

void UNameBox::InitializeTestData()
{
    // 데이터를 Map으로 관리
    MyDataMap.Add("aa", "350 5th Ave, New York, NY 10118, USA");
    MyDataMap.Add("ab", "Champ de Mars, 5 Avenue Anatole France, 75007 Paris, France");
    MyDataMap.Add("ac", "Piazza del Colosseo, 1, 00184 Roma RM, Italy");
}

FString* UNameBox::FindDataByName(FString Name)
{
    // Map에서 이름으로 데이터 검색
    return MyDataMap.Find(Name);
}

void UNameBox::DisplayDataInViewport(FString Name, FString Address)
{
    // 뷰포트에 데이터 표시
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
        FString::Printf(TEXT("Building Name: %s\nAddress: %s"), *Name, *Address));
}