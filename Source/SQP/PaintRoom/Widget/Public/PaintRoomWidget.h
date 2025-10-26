// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PaintRoomWidget.generated.h"

class UWidgetSwitcher;
class USelectedPRSInfoWidget;
class UEditableTextBox;
class UButton;

UCLASS()
class SQP_API UPaintRoomWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UWidgetSwitcher> WidgetSwitcher;

#pragma region 캔버스 판넬 1

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> OpenSavePanelButton;

#pragma endregion 

#pragma region 캔버스 판넬 2
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> SaveButton;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UEditableTextBox> SaveNameTextBox;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> BackPaintRoomButton1;

#pragma endregion 

#pragma region 캔버스 판넬 3
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<USelectedPRSInfoWidget> SelectedPRSInfoWidget;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> BackMainMenuButton1;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> BackPaintRoomButton2;

#pragma endregion

#pragma region 캔버스 판넬 4
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> BackMainMenuButton2;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> BackPaintRoomButton3;

#pragma endregion 

protected:
	UFUNCTION()
	void OnOpenSaveButtonClicked();

	UFUNCTION()
	void OnSaveButtonClicked();

	UFUNCTION()
	void OnBackMainMenuButtonClicked();

	UFUNCTION()
	void OnBackPaintRoomButtonClicked();
};
