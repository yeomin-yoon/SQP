// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Synth2DSlider.h"
#include "MainUI.generated.h"

/**
 * 
 */
UCLASS()
class SQP_API UMainUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	FLinearColor GetBorderColor();

protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class USynth2DSlider> ColorWheelSlider;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UBorder> CurrentColor;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UButton> SetColorRedBtn;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UButton> SetColorGreenBtn;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UButton> SetColorBlueBtn;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UButton> SetColorWhiteBtn;

	float GetAngle();
	bool IsInRadius();
	FLinearColor UpdateColorWheel();

	UFUNCTION()
	void OnClickRedBtn();
	UFUNCTION()
	void OnClickGreenBtn();
	UFUNCTION()
	void OnClickBlueBtn();
	UFUNCTION()
	void OnClickWhiteBtn();
	void SetColor(FLinearColor Color);
	FLinearColor GetColor();

	UFUNCTION()
	void OnSliderXChanged(float Value);
};
