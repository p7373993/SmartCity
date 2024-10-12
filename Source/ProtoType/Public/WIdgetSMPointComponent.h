// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Module/SMPointComponent.h"
#include "WIdgetSMPointComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE_API UWIdgetSMPointComponent : public USMPointComponent
{
	GENERATED_BODY()
	
public:
    void CallProtectedFunction(bool On)
    {
        if (On)
        {
            TempChangeType(EVisibleType::None);
        }
        else
        {
            TempChangeType(EVisibleType::Price);
        }
    }
};
