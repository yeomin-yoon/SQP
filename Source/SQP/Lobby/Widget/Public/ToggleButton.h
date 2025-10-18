// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "ToggleButton.generated.h"

class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnToggleStateChanged, bool, Value);

UCLASS()
class SQP_API UToggleButton : public UButton
{
	GENERATED_BODY()

public:
	UToggleButton();
	
	UPROPERTY(BlueprintAssignable, Category = "ToggleButton")
	FOnToggleStateChanged OnToggleStateChanged;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ToggleButton")
	FLinearColor ToggleOnColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ToggleButton")
	FLinearColor ToggleOffColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ToggleButton")
	FText ToggleOnText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ToggleButton")
	FText ToggleOffText;

	UFUNCTION(BlueprintCallable, Category = "ToggleButton")
	void SetToggle(bool Value);

	UFUNCTION(BlueprintPure, Category = "ToggleButton")
	FORCEINLINE bool IsToggled() const { return bToggle; }

protected:
	virtual void OnWidgetRebuilt() override;
	
	virtual void SynchronizeProperties() override;
	
	virtual void PostLoad() override;

	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly, Category = "ToggleButton", meta = (DisplayName = "Text Block"))
	TObjectPtr<UTextBlock> ButtonTextBlock;
    
private:
	UPROPERTY(VisibleAnywhere, Category = "ToggleButton")
	bool bToggle;

	UFUNCTION()
	void HandleClick();

	UFUNCTION()
	void UpdateButtonAppearance();
};