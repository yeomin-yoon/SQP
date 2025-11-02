// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlaygroundMenuWidget.generated.h"

class UWidgetSwitcher;
class UButton;

UCLASS()
class SQP_API UPlaygroundMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void OnMenuButtonClicked();

	UFUNCTION()
	void OnBackMainButtonClicked();

	UFUNCTION()
	void OnBackPaintButtonClicked();

protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UWidgetSwitcher> WidgetSwitcher; 
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> MenuButton;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> BackMainButton;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> BackPaintButton;
};
