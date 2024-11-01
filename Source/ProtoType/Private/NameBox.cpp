#include "NameBox.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Character/SMCharacterMoveComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Character/SMCharacter.h"

void UNameBox::NativeConstruct()
{
    Super::NativeConstruct();

    ///'/Game/Kichan/WB_Inform.WB_Inform'
     // 런타임에 WB_inform 클래스를 로드합니다.
   // WBInformClass = LoadClass<UUserWidget>(nullptr, TEXT("/Game/Kichan/WB_Inform.WB_Inform_C"));

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
    //정보 창 생성
    UInfomBox* InformBox = CreateWidget<UInfomBox>(GetWorld(), UInfomBox::StaticClass());
    if (InformBox)
    {
        FString NameTextValue = NameText->GetText().ToString();
        // 데이터를 전달하여 위젯을 생성하고 뷰포트에 추가
        InformBox->DisplayInformWidget(NameTextValue, "Address");
        //아파트 인덱스를 이용한 테스트 용
        //InformBox->DisplayInformWidget(1);
    }

    //검색한 건물 위치로 이동
    //위치이동하는 함수 호출 후, 현재 위젯에 담겨진 xpos, ypos의 값을 넘겨주면 된다.
    //UE_LOG(LogTemp, Warning, TEXT("My xPos is: %d"), xPos);
    //UE_LOG(LogTemp, Warning, TEXT("My yPos is: %d"), yPos);
    FVector TargetPosition(xPos, yPos, 80000.0f); // zpos는 고정값 6000

    // StartMovingToLocation 함수를 호출하여 목표 위치로 이동을 시작합니다.
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASMCharacter::StaticClass(), FoundActors);

    if (FoundActors.Num() > 0)
    {
        // 첫 번째 SMCharacter를 선택
        ASMCharacter* Character = Cast<ASMCharacter>(FoundActors[0]);

        if (Character)
        {
            // SMCharacter에서 USMPointComponent를 찾음
            USMCharacterMoveComponent* MoveComponent = Character->FindComponentByClass<USMCharacterMoveComponent>();

            if (MoveComponent)
            {
                MoveComponent->StartMovingToLocation(TargetPosition);
            }
        }
    }

    //// 데이터 검색
    //FString* FoundData = FindDataByName(NameTextValue);
    //if (FoundData)
    //{
    //    DisplayDataInViewport(NameTextValue, *FoundData);
    //}
}

////임시 건물 데이터
//void UNameBox::InitializeTestData()
//{
//    // 데이터를 Map으로 관리
//    MyDataMap.Add("aa", "350 5th Ave, New York, NY 10118, USA");
//    MyDataMap.Add("ab", "Champ de Mars, 5 Avenue Anatole France, 75007 Paris, France");
//    MyDataMap.Add("ac", "Piazza del Colosseo, 1, 00184 Roma RM, Italy");
//}
//
//FString* UNameBox::FindDataByName(FString Name)
//{
//    // Map에서 이름으로 데이터 검색
//    return MyDataMap.Find(Name);
//}
//
//
//void UNameBox::DisplayDataInViewport(FString Name, FString Address)
//{
//    if (WBInformClass)
//    {
//        // WB_inform 위젯을 생성합니다.
//        UUserWidget* InformWidget = CreateWidget<UUserWidget>(GetWorld(), WBInformClass);
//
//        if (InformWidget)
//        {
//            // 위젯에서 BuildingName과 BuildingAddress에 접근하여 텍스트 설정
//            UTextBlock* NameTextBlock = Cast<UTextBlock>(InformWidget->GetWidgetFromName("BuildingName"));
//            UTextBlock* AddressTextBlock = Cast<UTextBlock>(InformWidget->GetWidgetFromName("BuildingAddress"));
//
//            if (NameTextBlock && AddressTextBlock)
//            {
//                NameTextBlock->SetText(FText::FromString(Name));
//                AddressTextBlock->SetText(FText::FromString(Address));
//            }
//
//            // 위젯을 뷰포트에 추가합니다.
//            InformWidget->AddToViewport();
//        }
//    }
//}