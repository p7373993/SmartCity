#include "NameBox.h"
#include "Components/TextBlock.h"

void UNameBox::NativeConstruct()
{
    Super::NativeConstruct();

    // NameText�� BindWidget�� ����Ͽ� �ڵ����� ���ε��˴ϴ�.
    // BindWidget�� ������� �ʰ� �������� ã�� �ʹٸ� �Ʒ� �ڵ带 ����� �� �ֽ��ϴ�.
    if (!NameText)
    {
        NameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("NameTextBox")));
    }

    // NameText�� ���������� ���ε��Ǿ����� Ȯ��
    if (NameText)
    {
        // �ʱ� �۾��̳� ������ �ʿ��ϸ� ���⿡ �߰�
    }
}

void UNameBox::SetName(const FString& InName)
{
    if (NameText)
    {
        NameText->SetText(FText::FromString(InName));
    }
}
