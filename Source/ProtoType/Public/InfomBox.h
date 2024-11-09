#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InfomBox.generated.h"

UCLASS()
class PROTOTYPE_API UInfomBox : public UUserWidget
{
    GENERATED_BODY()

public:
    static UInfomBox* GetInstance(UObject* WorldContextObject);

    virtual void NativeConstruct() override;

    // 자식 블루프린트를 참조할 변수
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UInfomBox> InformBoxChildClass;

    //remove_btn

    // 다른 클래스에서 호출할 함수
    UFUNCTION(BlueprintCallable, Category = "UI")
    void DisplayInformWidget(const FString& BuildingName, const FString& BuildingAddress);

    void DisplayInformWidget(const int Bulidingindex, const int tag);

protected:
    // 자식 블루프린트 위젯을 생성하는 내부 함수
    UInfomBox* CreateChildInformWidget(const FString& BuildingName, const FString& BuildingAddress);

    // BuildingName과 BuildingAddress를 설정하는 함수
    UFUNCTION(BlueprintCallable, Category = "UI")
    virtual void SetBuildingInfo(const FString& BuildingName, const FString& BuildingAddress);

    UPROPERTY()
    class UButton* RemoveBtn;

    UFUNCTION()
    void RemoveBtnClicked();

    UInfomBox* cChildWidget;

private:
    static UInfomBox* Instance;

};
