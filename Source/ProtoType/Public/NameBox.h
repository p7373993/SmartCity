// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InfomBox.h"
#include "ProtoType/Global/Structs.h"
#include "NameBox.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE_API UNameBox : public UUserWidget
{
	GENERATED_BODY()

    int xPos;
    int yPos;

public:

    virtual void NativeConstruct() override;

    void setXpos(int x);
    void setYpos(int y);
    int getXpos();
    int getYpos();

    //UPROPERTY(meta = (BindWidget)) �̸��� ������ �ڵ����� ���ε�
    UPROPERTY()
    class UTextBlock* NameText;

    UPROPERTY()
    class UButton* NameBtn;

    SearchStruct BuildingData;

    // �̸��� �����ϴ� �Լ�
    void SetName(const FString& InName);

    //// ������ �ʱ�ȭ �Լ�
    //void InitializeTestData();

    //// �̸����� �����͸� �˻��ϴ� �Լ�
    //FString* FindDataByName(FString Name);

    //// �����͸� ����Ʈ�� ǥ���ϴ� �Լ�
    //void DisplayDataInViewport(FString Name, FString Address);

    // NameBtn�� Ŭ���Ǿ��� �� ȣ��Ǵ� �Լ�
    UFUNCTION()
    void OnNameBtnClicked();

    //TMap<FString, FString> MyDataMap;

//protected:
//    UPROPERTY(EditAnywhere, Category = "UI")
//    TSubclassOf<UUserWidget> WBInformClass;

//protected:
//    // UInformClass�� �������Ʈ Ŭ������ �����ϴ� ����
//    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
//    TSubclassOf<UInfomBox> InformClass;
//
//    // ���� ������ (���Ƿ� ����)
//    FString SampleBuildingName = TEXT("Sample Building");
//    FString SampleBuildingAddress = TEXT("123 Sample St.");
	
};
