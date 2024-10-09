// Fill out your copyright notice in the Description page of Project Settings.


#include "SearchBox.h"
#include "Components/EditableTextBox.h"
#include "Components/EditableText.h"
#include "Components/PanelWidget.h"
#include "Components/ScrollBox.h"

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

    if (SearchInput)
    {
        // �˻� �ؽ�Ʈ�� ����� �� ȣ��� ��������Ʈ ���ε�
        SearchInput->OnTextChanged.AddDynamic(this, &USearchBox::OnSearchTextChanged);
    }
}

void USearchBox::OnSearchTextChanged(const FText& Text)
{
    // ���� �˻� ��� �ʱ�ȭ
    SearchResults->ClearChildren();

    // �˻�� �´� ��� ���͸�
    TArray<FString> FilteredResults = GetFilteredResults(Text.ToString());

    // �������Ʈ���� ������ W_NameBox Ŭ������ �����ϴ��� Ȯ��
    if (NameTextBoxWidgetClass)
    {
        for (const FString& Result : FilteredResults)
        {
            // W_NameBox ���� (UNameBox�� �ڽ��̹Ƿ� UNameBox�� ĳ���� ����)
            UNameBox* NameTextBox = CreateWidget<UNameBox>(this, NameTextBoxWidgetClass);
            if (NameTextBox)
            {
                // ScrollBox�� �߰�
                SearchResults->AddChild(NameTextBox);

                // SetName �Լ��� �̸� �ؽ�Ʈ ����
                NameTextBox->SetName(Result);

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
        /* �����͸� �ҷ����� ���� */ 
        "aa","ab","ac","ba"
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

