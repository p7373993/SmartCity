// Fill out your copyright notice in the Description page of Project Settings.


#include "SearchBox.h"
#include "Components/EditableTextBox.h"
#include "Components/PanelWidget.h"
#include "Components/ScrollBox.h"
#include "NameBox.h"

void USearchBox::NativeConstruct()
{
    Super::NativeConstruct();

    if (!SearchInput)
    {
        SearchInput = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("SearchBar")));
    }
    if (!SearchResults)
    {
        SearchResults = Cast<UScrollBox>(GetWidgetFromName(TEXT("NamesList")));
    }

    if (SearchInput)
    {
        // 검색 텍스트가 변경될 때 호출될 델리게이트 바인딩
        SearchInput->OnTextChanged.AddDynamic(this, &USearchBox::OnSearchTextChanged);
    }
}

void USearchBox::OnSearchTextChanged(const FText& Text)
{
    // 기존 검색 결과를 초기화
    SearchResults->ClearChildren();

    // 검색어에 따라 필터링된 데이터를 반복 처리
    TArray<FString> FilteredResults = GetFilteredResults(Text.ToString());

    for (const FString& Result : FilteredResults)
    {
        // NameBox 위젯 생성 및 이름 설정
        UNameBox* NameBox = CreateWidget<UNameBox>(this, UNameBox::StaticClass());
        NameBox->SetName(Result);

        // 검색 결과 패널에 추가
        SearchResults->AddChild(NameBox);
    }
}

TArray<FString> USearchBox::GetFilteredResults(const FString& SearchText)
{
    TArray<FString> AllNames;
    AllNames = 
    {
        /* 데이터를 불러오는 로직 */ 
        "aa","ab","ac","ba","bb","bc","ca","cb","cc"
    };
    TArray<FString> FilteredNames;

    // 검색어에 맞는 이름 필터링
    for (const FString& Name : AllNames)
    {
        if (Name.Contains(SearchText))
        {
            FilteredNames.Add(Name);
        }
    }

    return FilteredNames;
}

