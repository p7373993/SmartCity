#include "NameBox.h"
#include "Components/TextBlock.h"

void UNameBox::NativeConstruct()
{
    Super::NativeConstruct();

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
}

void UNameBox::SetName(const FString& InName)
{
    if (NameText)
    {
        NameText->SetText(FText::FromString(InName));
    }
}
