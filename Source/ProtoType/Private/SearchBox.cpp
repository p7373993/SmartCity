// Fill out your copyright notice in the Description page of Project Settings.


#include "SearchBox.h"
#include "Components/EditableTextBox.h"
#include "Components/EditableText.h"
#include "Components/PanelWidget.h"
#include "Components/ScrollBox.h"
#include "ClientModule/TCPModule.h"
#include "Components/Button.h"


void USearchBox::NativeConstruct()
{
    Super::NativeConstruct();

    if (!SearchInput)
    {
        SearchInput = Cast<UEditableText>(GetWidgetFromName(TEXT("SearchBar")));
    }
    if (!SearchResults)
    {
        SearchResults = Cast<UScrollBox>(GetWidgetFromName(TEXT("NamesList")));
    }

    if (!ClearBtn)
    {
        ClearBtn = Cast<UButton>(GetWidgetFromName(TEXT("clear_btn")));
    }
    if (ClearBtn)
    {
        ClearBtn->OnClicked.AddDynamic(this, &USearchBox::OnClearBtnClicked);
    }

    if (SearchInput)
    {
        // 검색 텍스트가 변경될 때 호출될 델리게이트 바인딩
        SearchInput->OnTextChanged.AddDynamic(this, &USearchBox::OnSearchTextChanged);
    }
}

void USearchBox::OnSearchTextChanged(const FText& Text)
{
    // 기존 검색 결과 초기화
    SearchResults->ClearChildren();
    TCPModule& SearchTCPModule = TCPModule::GetInstance();
    // 검색어에 맞는 결과 필터링
    //Add TCP Moudule and create TArray<FString> FilteredResults = TCPModule.GetText()
    TArray<SearchStruct> FilteredResults = SearchTCPModule.SearchBuildingData(Text.ToString(),2);

    if (FilteredResults.Num() == 0)
    {

    }

    // 블루프린트에서 설정된 W_NameBox 클래스를 참조하는지 확인
    if (NameTextBoxWidgetClass)
    {
        for (const SearchStruct& Result : FilteredResults)
        {
            // W_NameBox 생성 (UNameBox의 자식이므로 UNameBox로 캐스팅 가능)
            UNameBox* NameTextBox = CreateWidget<UNameBox>(this, NameTextBoxWidgetClass);
            if (NameTextBox)
            {
                // ScrollBox에 추가
                SearchResults->AddChild(NameTextBox);

                // SetName 함수로 이름 텍스트 설정
                NameTextBox->SetName(Result.BuildingName);
                //임의의 건물 위치 전달
                NameTextBox->setXpos(10);
                NameTextBox->setYpos(10);
                NameTextBox->BuildingData = Result;
                NameTextBox->SetVisibility(ESlateVisibility::Visible);
            }
        }
    }
}



TArray<FString> USearchBox::GetFilteredResults(const FString& SearchText)
{
    TArray<FString> AllNames;
    AllNames = 
    {
        /* 데이터를 불러오는 로직 */ 
        "aa","ab","ac","ba"
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

void USearchBox::OnClearBtnClicked()
{
    if (SearchInput)
    {
        SearchInput->SetText(FText::GetEmpty());
    }
}

