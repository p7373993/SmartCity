// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProtoType/Sungwoo/MyCharacter.h"
#include "RayCastTest001.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE_API ARayCastTest001 : public AMyCharacter
{
	GENERATED_BODY()
	virtual void Tick(float DeltaTime)override;

	ARayCastTest001();


	virtual void GetPoint(FVector2D LU, FVector2D LD, FVector2D RU, FVector2D RD) override;

	UFUNCTION(BlueprintCallable, Category = "View")
	virtual void RayCast(const FVector& StartLocation, const FVector& EndLocation,int32 FloorInfo = 10) override;

	void ChangeBuildingMaterial(FHitResult& HitResult, FLinearColor InNewColor);
	void ChangeMaterialRGB(USkeletalMeshComponent* SkeletalMesh, FName BoneName, FLinearColor NewColor);



	UMaterialInterface* InstMaterial;

public:
	FLinearColor GetSpectrumColor(float Value, int32 MaxValue);//»ö ½ºÆåÆ®·³
	void latLongToXY(double latitude, double longitude, double& x, double& y);
	void XYTolatLong(double x, double y, double& latitude, double& longitude);

	//UFUNCTION(BlueprintCallable, Category = "View")
	//void TempRaycast();
};
