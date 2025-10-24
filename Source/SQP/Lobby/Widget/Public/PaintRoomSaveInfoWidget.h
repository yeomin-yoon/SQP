// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PaintRoomSaveInfoWidget.generated.h"

class UTextBlock;

UCLASS()
class SQP_API UPaintRoomSaveInfoWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> SaveNameTextBlock;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> SaveIDTextBlock;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> SaveDateTextBlock;
};
