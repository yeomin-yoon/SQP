// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SQP_SG_Main.h"
#include "Blueprint/UserWidget.h"
#include "SelectedPRSInfoWidget.generated.h"

class UTextBlock;

UCLASS()
class SQP_API USelectedPRSInfoWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> SaveNameTextBlock;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> SaveIDTextBlock;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> SaveDateTextBlock;

public:
	UFUNCTION()
	void UpdatePRSInfo(const FSQP_PainRoomSaveFormat& PainRoomSave) const;
};
