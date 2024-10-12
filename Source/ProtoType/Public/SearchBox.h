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
    // 검색 입력 필드를 위한 변수
    UPROPERTY()
    class UEditableText* SearchInput;

    // 검색 결과 리스트를 위한 변수
    UPROPERTY()
    class UScrollBox* SearchResults;

    virtual void NativeConstruct() override;

    // 검색을 처리하는 함수
    UFUNCTION()
    void OnSearchTextChanged(const FText& Text);

private:
    // 검색 결과를 필터링하는 함수의 선언
    TArray<FString> GetFilteredResults(const FString& SearchText);

protected:
    // USearchBox.h
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
    TSubclassOf<UNameBox> NameTextBoxWidgetClass;
};
