#include "NameBox.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UNameBox::NativeConstruct()
{
    Super::NativeConstruct();

    ///'/Game/Kichan/WB_Inform.WB_Inform'
     // ��Ÿ�ӿ� WB_inform Ŭ������ �ε��մϴ�.
   // WBInformClass = LoadClass<UUserWidget>(nullptr, TEXT("/Game/Kichan/WB_Inform.WB_Inform_C"));

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
   // InitializeTestData();
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

    UInfomBox* InformBox = CreateWidget<UInfomBox>(GetWorld(), UInfomBox::StaticClass());
    if (InformBox)
    {
        FString NameTextValue = NameText->GetText().ToString();
        // �����͸� �����Ͽ� ������ �����ϰ� ����Ʈ�� �߰�
        InformBox->DisplayInformWidget(NameTextValue, "Address");
        //����Ʈ �ε����� �̿��� �׽�Ʈ ��
        //InformBox->DisplayInformWidget(1);
    }

    

    //// ������ �˻�
    //FString* FoundData = FindDataByName(NameTextValue);
    //if (FoundData)
    //{
    //    DisplayDataInViewport(NameTextValue, *FoundData);
    //}
}

////�ӽ� �ǹ� ������
//void UNameBox::InitializeTestData()
//{
//    // �����͸� Map���� ����
//    MyDataMap.Add("aa", "350 5th Ave, New York, NY 10118, USA");
//    MyDataMap.Add("ab", "Champ de Mars, 5 Avenue Anatole France, 75007 Paris, France");
//    MyDataMap.Add("ac", "Piazza del Colosseo, 1, 00184 Roma RM, Italy");
//}
//
//FString* UNameBox::FindDataByName(FString Name)
//{
//    // Map���� �̸����� ������ �˻�
//    return MyDataMap.Find(Name);
//}
//
//
//void UNameBox::DisplayDataInViewport(FString Name, FString Address)
//{
//    if (WBInformClass)
//    {
//        // WB_inform ������ �����մϴ�.
//        UUserWidget* InformWidget = CreateWidget<UUserWidget>(GetWorld(), WBInformClass);
//
//        if (InformWidget)
//        {
//            // �������� BuildingName�� BuildingAddress�� �����Ͽ� �ؽ�Ʈ ����
//            UTextBlock* NameTextBlock = Cast<UTextBlock>(InformWidget->GetWidgetFromName("BuildingName"));
//            UTextBlock* AddressTextBlock = Cast<UTextBlock>(InformWidget->GetWidgetFromName("BuildingAddress"));
//
//            if (NameTextBlock && AddressTextBlock)
//            {
//                NameTextBlock->SetText(FText::FromString(Name));
//                AddressTextBlock->SetText(FText::FromString(Address));
//            }
//
//            // ������ ����Ʈ�� �߰��մϴ�.
//            InformWidget->AddToViewport();
//        }
//    }
//}