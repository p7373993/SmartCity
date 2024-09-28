// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_smMain.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Character/SMCharacter.h"
#include "Module/SMPointComponent.h"
#include "WIdgetSMPointComponent.h"

void UUW_smMain::NativeConstruct()
{
	Super::NativeConstruct();
	Btn_ChangeColor = Cast<UButton>(GetWidgetFromName(TEXT("Btn_1")));
	Btn_AddLandmark = Cast<UButton>(GetWidgetFromName(TEXT("Btn_2")));
	Btn_ShowingMenu = Cast<UButton>(GetWidgetFromName(TEXT("Btn_3")));
	if (Btn_ChangeColor)
	{
		Btn_ChangeColor->OnClicked.AddDynamic(this, &UUW_smMain::ColorChangeButtonClicked);
	}
	if (Btn_ChangeColor)
	{
		Btn_ChangeColor->OnClicked.AddDynamic(this, &UUW_smMain::AddLandmarkButtonClicked);
	}
	if (Btn_ChangeColor)
	{
		Btn_ChangeColor->OnClicked.AddDynamic(this, &UUW_smMain::ShowingMenuButtonClicked);
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
				}
				else
				{
					SMPointComponent->TempChangeType(EVisibleType::Price); // USMPointComponent의 public 함수 호출
					Btn_1On = true;
				}
			}
		}
	}


	//USMPointComponent* component;
	
}

void UUW_smMain::AddLandmarkButtonClicked()
{

}

void UUW_smMain::ShowingMenuButtonClicked()
{

}
