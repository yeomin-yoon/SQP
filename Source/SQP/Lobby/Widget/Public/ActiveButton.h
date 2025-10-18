// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "ActiveButton.generated.h"

class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActiveStateChanged, bool, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActiveButtonClicked);

UCLASS()
class SQP_API UActiveButton : public UButton
{
	GENERATED_BODY()

public:
	UActiveButton();
	
	UPROPERTY(BlueprintAssignable, Category = "ActiveButton")
	FOnActiveStateChanged OnActiveStateChanged;

	UPROPERTY(BlueprintAssignable, Category = "ActiveButton")
	FOnActiveButtonClicked OnActiveButtonClicked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActiveButton")
	FLinearColor ActiveOnColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActiveButton")
	FLinearColor ActiveOffColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActiveButton")
	FText ActiveOnText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActiveButton")
	FText ActiveOffText;

	UFUNCTION(BlueprintCallable, Category = "ActiveButton")
	void SetActive(bool Value);

	UFUNCTION(BlueprintPure, Category = "ActiveButton")
	FORCEINLINE bool IsActive() const { return bActive; }

protected:
	virtual void OnWidgetRebuilt() override;
	
	virtual void SynchronizeProperties() override;
	
	virtual void PostLoad() override;

	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly, Category = "ActiveButton", meta = (DisplayName = "Text Block"))
	TObjectPtr<UTextBlock> ButtonTextBlock;
    
private:
	UPROPERTY(EditAnywhere, Category = "ActiveButton")
	bool bActive;

	UFUNCTION()
	void HandleClick();
	
	UFUNCTION()
	void UpdateButtonAppearance();
};