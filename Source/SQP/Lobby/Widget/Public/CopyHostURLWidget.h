// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CopyHostURLWidget.generated.h"

class UTextBlock;
class UButton;

UCLASS()
class SQP_API UCopyHostURLWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> CopyHostURLButton;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> URLTextBlock;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> CopyNotificationTextBlock;

	FTimerHandle HideNotificationTimerHandle;
	
	UFUNCTION()
	void OnCopyHostURLButtonClicked();

protected:
	UPROPERTY()
	FString CopiedHostURL;
};