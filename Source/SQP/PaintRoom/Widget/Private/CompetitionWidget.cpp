// Fill out your copyright notice in the Description page of Project Settings.


#include "CompetitionWidget.h"

#include "Components/TextBlock.h"

void UCompetitionWidget::NativeDestruct()
{
	Super::NativeDestruct();

	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

void UCompetitionWidget::ShowSomeoneWin(const FString& SomeoneName)
{
	CorrectTextBlock->SetVisibility(ESlateVisibility::Visible);
	CorrectTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%s님이 정답을 맞추셨습니다!"), *SomeoneName)));

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
	{
		CorrectTextBlock->SetVisibility(ESlateVisibility::Hidden);
	}, 3, false);
}

void UCompetitionWidget::ShowWin(const FString& MyName)
{
	CorrectTextBlock->SetVisibility(ESlateVisibility::Visible);
	CorrectTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%s님 정답입니다!"), *MyName)));

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
	{
		CorrectTextBlock->SetVisibility(ESlateVisibility::Hidden);
	}, 3, false);
}

void UCompetitionWidget::HideAll() const
{
	CorrectTextBlock->SetVisibility(ESlateVisibility::Hidden);
}
