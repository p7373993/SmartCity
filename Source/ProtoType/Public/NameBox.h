// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NameBox.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE_API UNameBox : public UUserWidget
{
	GENERATED_BODY()

public:

    virtual void NativeConstruct() override;

    //UPROPERTY(meta = (BindWidget)) 이름이 같을시 자동으로 바인딩
    UPROPERTY()
    class UTextBlock* NameText;

    // 이름을 설정하는 함수
    void SetName(const FString& InName);
	
};
