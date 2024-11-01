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

    // �ڽ� �������Ʈ�� ������ ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UInfomBox> InformBoxChildClass;

    //remove_btn

    // �ٸ� Ŭ�������� ȣ���� �Լ�
    UFUNCTION(BlueprintCallable, Category = "UI")
    void DisplayInformWidget(const FString& BuildingName, const FString& BuildingAddress);

    void DisplayInformWidget(const int Bulidingindex, const int tag);

protected:
    // �ڽ� �������Ʈ ������ �����ϴ� ���� �Լ�
    UInfomBox* CreateChildInformWidget(const FString& BuildingName, const FString& BuildingAddress);

    // BuildingName�� BuildingAddress�� �����ϴ� �Լ�
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
