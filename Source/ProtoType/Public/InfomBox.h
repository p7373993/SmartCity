#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InfomBox.generated.h"

UCLASS()
class PROTOTYPE_API UInfomBox : public UUserWidget
{
    GENERATED_BODY()

public:
    // ������ �����ϰ� �����͸� ����Ʈ�� ǥ���ϴ� �Լ�
    UFUNCTION(BlueprintCallable, Category = "Inform")
    static UInfomBox* CreateAndDisplay(UObject* WorldContextObject, TSubclassOf<UInfomBox> InformClass, FString BuildingName, FString BuildingAddress);

protected:
    // �����͸� �����ϴ� ���� �Լ�
    void DisplayData(FString BuildingName, FString BuildingAddress);

    // TextBlock ������
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* BuildingNameText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* BuildingAddressText;
};
