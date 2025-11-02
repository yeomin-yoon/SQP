// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlaygroundPlayerScoreWidget.generated.h"

class UTextBlock;

UCLASS()
class SQP_API UPlaygroundPlayerScoreWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> NameTextBlock;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> IDTextBlock;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> ScoreTextBlock;

	UFUNCTION()
	void Init(const FString& Name, const FString& UniqueId, const FColor& Color, const int32& Score) const;

	UFUNCTION()
	void SetScore(const int32& Score) const;
};
