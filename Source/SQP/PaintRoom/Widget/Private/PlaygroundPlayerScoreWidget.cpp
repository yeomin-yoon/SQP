// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaygroundPlayerScoreWidget.h"

#include "Components/TextBlock.h"

void UPlaygroundPlayerScoreWidget::Init(const FString& Name, const FString& UniqueId, const FColor& Color, const int32& Score) const
{
	NameTextBlock->SetText(FText::FromString(Name));
	IDTextBlock->SetText(FText::FromString(UniqueId));
	ScoreTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%d"), Score)));
	NameTextBlock->SetColorAndOpacity(Color);
	IDTextBlock->SetColorAndOpacity(Color);
	ScoreTextBlock->SetColorAndOpacity(Color);
}

void UPlaygroundPlayerScoreWidget::SetScore(const int32& Score) const
{
	ScoreTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%d"), Score)));
}

