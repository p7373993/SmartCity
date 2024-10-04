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
        // �˻� �ؽ�Ʈ�� ����� �� ȣ��� ��������Ʈ ���ε�
        SearchInput->OnTextChanged.AddDynamic(this, &USearchBox::OnSearchTextChanged);
    }
}

void USearchBox::OnSearchTextChanged(const FText& Text)
{
    // ���� �˻� ����� �ʱ�ȭ
    SearchResults->ClearChildren();

    // �˻�� ���� ���͸��� �����͸� �ݺ� ó��
    TArray<FString> FilteredResults = GetFilteredResults(Text.ToString());

    for (const FString& Result : FilteredResults)
    {
        // NameBox ���� ���� �� �̸� ����
        UNameBox* NameBox = CreateWidget<UNameBox>(this, UNameBox::StaticClass());
        NameBox->SetName(Result);

        // �˻� ��� �гο� �߰�
        SearchResults->AddChild(NameBox);
    }
}

TArray<FString> USearchBox::GetFilteredResults(const FString& SearchText)
{
    TArray<FString> AllNames;
    AllNames = 
    {
        /* �����͸� �ҷ����� ���� */ 
        "aa","ab","ac","ba","bb","bc","ca","cb","cc"
    };
    TArray<FString> FilteredNames;

    // �˻�� �´� �̸� ���͸�
    for (const FString& Name : AllNames)
    {
        if (Name.Contains(SearchText))
        {
            FilteredNames.Add(Name);
        }
    }

    return FilteredNames;
}

