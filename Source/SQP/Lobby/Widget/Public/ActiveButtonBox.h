// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActiveButton.h"
#include "Components/VerticalBox.h"
#include "ActiveButtonBox.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class SQP_API UActiveButtonBox : public UVerticalBox
{
	GENERATED_BODY()

public:
	UActiveButtonBox();

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActiveButtonBox")
	FText OffConditionText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActiveButtonBox")
	FText OnConditionText;

	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly, Category = "ActiveButtonBox", meta = (DisplayName = "Button"))
	TObjectPtr<UButton> ActiveButton;

protected:
	virtual void OnWidgetRebuilt() override;
	
	virtual void SynchronizeProperties() override;
	
	virtual void PostLoad() override;

	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly, Category = "ActiveButtonBox", meta = (DisplayName = "Text Block"))
	TObjectPtr<UTextBlock> ConditionTextBlock;

	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly, Category = "ActiveButtonBox", meta = (DisplayName = "Text Block"))
	TObjectPtr<UTextBlock> ActiveButtonTextBlock;

private:
	UPROPERTY(EditAnywhere, Category = "ActiveButton")
	bool bActive;

	UFUNCTION()
	void HandleClick();
	
	UFUNCTION()
	void UpdateBoxAppearance();
};