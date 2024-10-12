// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Module/SMPointComponent.h"
#include "UW_smMain.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE_API UUW_smMain : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void ColorChangeButtonClicked();
	UFUNCTION()
	void AddLandmarkButtonClicked();
	UFUNCTION()
	void ShowingMenuButtonClicked();

protected:
	virtual void NativeConstruct() override;

private:
	bool Btn_1On = false;
	bool Btn_2On = false;
	bool Btn_3On = false;

	UPROPERTY()
	class UButton* Btn_ChangeColor;

	UPROPERTY()
	class UButton* Btn_AddLandmark;

	UPROPERTY()
	class UButton* Btn_ShowingMenu;

//protected:
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget Actor")
//	AWidgetActor* WidgetActor;  // WidgetActor ÂüÁ¶
	
};
