#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InfomBox.generated.h"

UCLASS()
class PROTOTYPE_API UInfomBox : public UUserWidget
{
    GENERATED_BODY()

public:
    // 위젯을 생성하고 데이터를 뷰포트에 표시하는 함수
    UFUNCTION(BlueprintCallable, Category = "Inform")
    static UInfomBox* CreateAndDisplay(UObject* WorldContextObject, TSubclassOf<UInfomBox> InformClass, FString BuildingName, FString BuildingAddress);

protected:
    // 데이터를 설정하는 내부 함수
    void DisplayData(FString BuildingName, FString BuildingAddress);

    // TextBlock 변수들
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* BuildingNameText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* BuildingAddressText;
};
