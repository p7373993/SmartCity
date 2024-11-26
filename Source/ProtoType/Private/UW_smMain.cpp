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
		// ù ��° SMCharacter�� ����
		ASMCharacter* Character = Cast<ASMCharacter>(FoundActors[0]);

		if (Character)
		{
			// SMCharacter���� USMPointComponent�� ã��
			USMPointComponent* SMPointComponent = Character->FindComponentByClass<USMPointComponent>();

			if (SMPointComponent)
			{
				if (Btn_1On)
				{
					SMPointComponent->TempChangeType(EVisibleType::None); // USMPointComponent�� public �Լ� ȣ��
					Btn_1On = false;

					if (ColorBoxInstance)
					{
						ColorBoxInstance->RemoveFromParent();
						ColorBoxInstance = nullptr; // �ν��Ͻ��� null�� �����Ͽ� �޸� ����
					}
				}
				else
				{

					SMPointComponent->TempChangeType(EVisibleType::Price); // USMPointComponent�� public �Լ� ȣ��
					Btn_1On = true;

					//���� �׷��� ��Ÿ����
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
	// ���� ���
	Btn_2On = !Btn_2On;

	OnButtonStateChanged.Broadcast(Btn_2On); // ��������Ʈ ȣ��
}

void UUW_smMain::ShowingMenuButtonClicked()
{

}
