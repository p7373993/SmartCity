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

    if (NameBtn)
    {
        NameBtn->OnClicked.AddDynamic(this, &UNameBox::OnNameBtnClicked);
    }

    // �׽�Ʈ�� ������ �ʱ�ȭ
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

    // ������ �˻�
    FString* FoundData = FindDataByName(NameTextValue);
    if (FoundData)
    {
        DisplayDataInViewport(NameTextValue, *FoundData);
    }
}

void UNameBox::InitializeTestData()
{
    // �����͸� Map���� ����
    MyDataMap.Add("aa", "350 5th Ave, New York, NY 10118, USA");
    MyDataMap.Add("ab", "Champ de Mars, 5 Avenue Anatole France, 75007 Paris, France");
    MyDataMap.Add("ac", "Piazza del Colosseo, 1, 00184 Roma RM, Italy");
}

FString* UNameBox::FindDataByName(FString Name)
{
    // Map���� �̸����� ������ �˻�
    return MyDataMap.Find(Name);
}

void UNameBox::DisplayDataInViewport(FString Name, FString Address)
{
    // ����Ʈ�� ������ ǥ��
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
        FString::Printf(TEXT("Building Name: %s\nAddress: %s"), *Name, *Address));
}