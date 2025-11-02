// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CompetitorName.generated.h"

/**
 * 
 */
UCLASS()
class SQP_API UCompetitorName : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UTextBlock> CompetitorName;
};
