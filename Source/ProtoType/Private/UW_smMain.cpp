// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_smMain.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Character/SMCharacter.h"
#include "Module/SMPointComponent.h"
#include "WIdgetSMPointComponent.h"


//UUW_smMain* UUW_smMain::Instance = nullptr;
//
//UUW_smMain* UUW_smMain::GetInstance(UObject* WorldContextObject)
//{
//	if (!Instance)
//	{
//		Instance = CreateWidget<UUW_smMain>(WorldContextObject->GetWorld(), UUW_smMain::StaticClass());
//	}
//
//	return Instance;
//}

void UUW_smMain::NativeConstruct()
{
	Super::NativeConstruct();
	/*/ Script / UMGEditor.WidgetBlueprint'/Game/Blueprints/ColorWidget.ColorWidget'*/
	Btn_ChangeColor = Cast<UButton>(GetWidgetFromName(TEXT("Btn_1")));
	Btn_AddLandmark = Cast<UButton>(GetWidgetFromName(TEXT("Btn_2")));
	Btn_ShowingMenu = Cast<UButton>(GetWidgetFromName(TEXT("Btn_3")));
	if (Btn_ChangeColor)
	{
		Btn_ChangeColor->OnClicked.AddDynamic(this, &UUW_smMain::ColorChangeButtonClicked);
	}
	if (Btn_AddLandmark)
	{
		Btn_AddLandmark->OnClicked.AddDynamic(this, &UUW_smMain::AddLandmarkButtonClicked);
	}
	if (Btn_ShowingMenu)
	{
		Btn_ShowingMenu->OnClicked.AddDynamic(this, &UUW_smMain::ShowingMenuButtonClicked);
	}
}


//void UUW_smMain::ColorChangeButtonClicked()
//{
//	AWidgetActor* OwnerActor = GetWorld()->SpawnActor<AWidgetActor>(AActor::StaticClass());
//	if (OwnerActor)
//	{
//		UWIdgetSMPointComponent* MyComponent = NewObject<UWIdgetSMPointComponent>(OwnerActor);
//		MyComponent->CallProtectedFunction(Btn_1On);
//		if (Btn_1On) Btn_1On = false;
//		else Btn_1On = true;
//	}
//}

void UUW_smMain::ColorChangeButtonClicked()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASMCharacter::StaticClass(), FoundActors);

	if (FoundActors.Num() > 0)
	{
		// 첫 번째 SMCharacter를 선택
		ASMCharacter* Character = Cast<ASMCharacter>(FoundActors[0]);

		if (Character)
		{
			// SMCharacter에서 USMPointComponent를 찾음
			USMPointComponent* SMPointComponent = Character->FindComponentByClass<USMPointComponent>();

			if (SMPointComponent)
			{
				if (Btn_1On)
				{
					SMPointComponent->TempChangeType(EVisibleType::None); // USMPointComponent의 public 함수 호출
					Btn_1On = false;

					if (ColorBoxInstance)
					{
						ColorBoxInstance->RemoveFromParent();
						ColorBoxInstance = nullptr; // 인스턴스를 null로 설정하여 메모리 관리
					}
				}
				else
				{

					SMPointComponent->TempChangeType(EVisibleType::Price); // USMPointComponent의 public 함수 호출
					Btn_1On = true;

					//색깔 그래프 나타내기
					if (ColorBoxWidgetClass)
					{
						ColorBoxInstance = CreateWidget<UUserWidget>(this, ColorBoxWidgetClass);
						if (ColorBoxInstance) ColorBoxInstance->AddToViewport();
					}
				}
			}
		}
	}


	//USMPointComponent* component;
	
}

void UUW_smMain::AddLandmarkButtonClicked()
{
	// 상태 토글
	Btn_2On = !Btn_2On;

	OnButtonStateChanged.Broadcast(Btn_2On); // 델리게이트 호출
}

void UUW_smMain::ShowingMenuButtonClicked()
{

}
