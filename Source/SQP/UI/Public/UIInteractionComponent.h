// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Components/WidgetInteractionComponent.h"
#include "UIInteractionComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SQP_API UUIInteractionComponent : public UWidgetInteractionComponent
{
	GENERATED_BODY()

public:
	UUIInteractionComponent();

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<APlayerController> PC;
	UPROPERTY()
	TObjectPtr<class UInputMappingContext> IMC;
	UPROPERTY()
	TObjectPtr<class UInputAction> RightClickAction;
	UFUNCTION()
	void OnRightClickPressed();
	void OnRightClickReleased();

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
