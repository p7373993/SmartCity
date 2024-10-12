// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetActor.h"
#include "WIdgetSMPointComponent.h"

// Sets default values
AWidgetActor::AWidgetActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// UWidgetSMPointComponent를 생성하고 이 캐릭터에 부착
	UWIdgetSMPointComponent* WidgetSMPointComponent = CreateDefaultSubobject<UWIdgetSMPointComponent>(TEXT("WidgetSMPointComponent"));
}

// Called when the game starts or when spawned
void AWidgetActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWidgetActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

