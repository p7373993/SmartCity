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

    //UPROPERTY(meta = (BindWidget)) �̸��� ������ �ڵ����� ���ε�
    UPROPERTY()
    class UTextBlock* NameText;

    // �̸��� �����ϴ� �Լ�
    void SetName(const FString& InName);
	
};
