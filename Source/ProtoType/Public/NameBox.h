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

    //UPROPERTY(meta = (BindWidget)) 이름이 같을시 자동으로 바인딩
    UPROPERTY()
    class UTextBlock* NameText;

    UPROPERTY()
    class UButton* NameBtn;

    SearchStruct BuildingData;

    // 이름을 설정하는 함수
    void SetName(const FString& InName);

    //// 데이터 초기화 함수
    //void InitializeTestData();

    //// 이름으로 데이터를 검색하는 함수
    //FString* FindDataByName(FString Name);

    //// 데이터를 뷰포트에 표시하는 함수
    //void DisplayDataInViewport(FString Name, FString Address);

    // NameBtn이 클릭되었을 때 호출되는 함수
    UFUNCTION()
    void OnNameBtnClicked();
    void latLongToXY(double latitude, double longitude, double& x, double& y);

private:
    UInfomBox* CurrentInformBox; // 현재 InformBox 인스턴스를 저장할 변수
    //TMap<FString, FString> MyDataMap;

//protected:
//    UPROPERTY(EditAnywhere, Category = "UI")
//    TSubclassOf<UUserWidget> WBInformClass;

//protected:
//    // UInformClass의 블루프린트 클래스를 지정하는 변수
//    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
//    TSubclassOf<UInfomBox> InformClass;
//
//    // 예시 데이터 (임의로 설정)
//    FString SampleBuildingName = TEXT("Sample Building");
//    FString SampleBuildingAddress = TEXT("123 Sample St.");
	
};
