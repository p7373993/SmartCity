// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NameBox.h"
#include "Blueprint/UserWidget.h"
#include "SearchBox.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE_API USearchBox : public UUserWidget
{
	GENERATED_BODY()
	
public:
    // �˻� �Է� �ʵ带 ���� ����
    UPROPERTY()
    class UEditableText* SearchInput;

    // �˻� ��� ����Ʈ�� ���� ����
    UPROPERTY()
    class UScrollBox* SearchResults;

    virtual void NativeConstruct() override;

    // �˻��� ó���ϴ� �Լ�
    UFUNCTION()
    void OnSearchTextChanged(const FText& Text);

private:
    // �˻� ����� ���͸��ϴ� �Լ��� ����
    TArray<FString> GetFilteredResults(const FString& SearchText);

protected:
    // USearchBox.h
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
    TSubclassOf<UNameBox> NameTextBoxWidgetClass;
};
