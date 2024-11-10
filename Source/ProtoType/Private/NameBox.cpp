#include "NameBox.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Character/SMCharacterMoveComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Character/SMCharacter.h"
#include "ProtoType/private/ClientModule/TCPModule.h"

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

void UNameBox::setXpos(int x)
{
    xPos = x;
}

void UNameBox::setYpos(int y)
{
    yPos = y;
}

int UNameBox::getXpos()
{
    return xPos;
}

int UNameBox::getYpos()
{
    return yPos;
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
    TCPModule& TCPModuleA = TCPModule::GetInstance();
    int Type = 1;
    float TempEL[20] = { Type,BuildingData.index,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
    UInfomBox* InformBox = CreateWidget<UInfomBox>(GetWorld(), UInfomBox::StaticClass());
    if (InformBox)
    {

        FString NameTextValue = NameText->GetText().ToString();
        TextStruct TempText = TCPModuleA.GetBuildingAddressAndName(TempEL,2);

        InformBox->DisplayInformWidget(TempText.BuildingName, TempText.BuildingAddress); // ������ ����
    }

    //�˻��� �ǹ� ��ġ�� �̵�
    //��ġ�̵��ϴ� �Լ� ȣ�� ��, ���� ������ ����� xpos, ypos�� ���� �Ѱ��ָ� �ȴ�.
    //UE_LOG(LogTemp, Warning, TEXT("My xPos is: %d"), xPos);
    //UE_LOG(LogTemp, Warning, TEXT("My yPos is: %d"), yPos);
    std::vector<float> XYpos = TCPModuleA.GetXYLocation(TempEL,2);
    double X = 0;
    double Y = 0;
    latLongToXY(XYpos[0], XYpos[1], X, Y);
    FVector TargetPosition(X, Y, 80000.0f); // zpos�� ������ 6000

    // StartMovingToLocation �Լ��� ȣ���Ͽ� ��ǥ ��ġ�� �̵��� �����մϴ�.
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASMCharacter::StaticClass(), FoundActors);

    if (FoundActors.Num() > 0)
    {
        // ù ��° SMCharacter�� ����
        ASMCharacter* Character = Cast<ASMCharacter>(FoundActors[0]);

        if (Character)
        {
            // SMCharacter���� USMPointComponent�� ã��
            USMCharacterMoveComponent* MoveComponent = Character->FindComponentByClass<USMCharacterMoveComponent>();

            if (MoveComponent)
            {
                MoveComponent->StartMovingToLocation(TargetPosition);
            }
        }
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
const double R = 6371000;
const double DEG_TO_RAD = PI / 180.0;

void UNameBox::latLongToXY(double latitude, double longitude, double& x, double& y)
{
    const double baseLatitude = 36.50476937;
    const double baseLongitude = 127.2784241;



    const double scaleX = -8691673.56;
    const double scaleY = -10995829.86;

    double phi0 = 36.50476937 * DEG_TO_RAD;
    double lambda0 = 127.2784241 * DEG_TO_RAD;
    double phi = latitude * DEG_TO_RAD;
    double lambda = longitude * DEG_TO_RAD;

    x = R * (lambda - lambda0) * cos(phi0) * 100 + 13167 - 3040 + 1000 + 1000;
    y = -R * (phi - phi0) * 100 + 3073 + 6597 - 6000 - 200;
}