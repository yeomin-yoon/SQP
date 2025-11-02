// Fill out your copyright notice in the Description page of Project Settings.


#include "CatchMindWidget.h"

#include "SQP_PC_PaintRoom.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"

void UCatchMindWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//정답 제출 텍스트 박스 바인딩
	AnswerTextBox->OnTextCommitted.AddDynamic(this, &UCatchMindWidget::OnAnswerTextCommitted);
}

void UCatchMindWidget::HideAll() const
{
	PainterRoleTextBlock->SetVisibility(ESlateVisibility::Hidden);
	ParticipantRoleTextBlock->SetVisibility(ESlateVisibility::Hidden);
	BigPainterRoleTextBlock->SetVisibility(ESlateVisibility::Hidden);
	BigParticipantRoleTextBlock->SetVisibility(ESlateVisibility::Hidden);
	SuggestionTitleTextBlock->SetVisibility(ESlateVisibility::Hidden);
	SuggestionTextBlock->SetVisibility(ESlateVisibility::Hidden);
	HintTitleTextBlock->SetVisibility(ESlateVisibility::Hidden);
	HintTextBlock->SetVisibility(ESlateVisibility::Hidden);
	WrongTextBlock->SetVisibility(ESlateVisibility::Hidden);
	CorrectTextBlock->SetVisibility(ESlateVisibility::Hidden);
	AnswerTextBox->SetVisibility(ESlateVisibility::Hidden);
	AnswerTextBox->SetIsEnabled(false);
	TimeUpTextBlock->SetVisibility(ESlateVisibility::Hidden);
}

void UCatchMindWidget::ShowPainter()
{
	PainterRoleTextBlock->SetVisibility(ESlateVisibility::Visible);
	ParticipantRoleTextBlock->SetVisibility(ESlateVisibility::Hidden);
	BigPainterRoleTextBlock->SetVisibility(ESlateVisibility::Visible);
	BigParticipantRoleTextBlock->SetVisibility(ESlateVisibility::Hidden);
	SuggestionTitleTextBlock->SetVisibility(ESlateVisibility::Visible);
	SuggestionTextBlock->SetVisibility(ESlateVisibility::Visible);
	HintTitleTextBlock->SetVisibility(ESlateVisibility::Visible);
	HintTextBlock->SetVisibility(ESlateVisibility::Visible);
	WrongTextBlock->SetVisibility(ESlateVisibility::Hidden);
	CorrectTextBlock->SetVisibility(ESlateVisibility::Hidden);
	AnswerTextBox->SetVisibility(ESlateVisibility::Hidden);
	AnswerTextBox->SetIsEnabled(false);
	TimeUpTextBlock->SetVisibility(ESlateVisibility::Hidden);

	GetWorld()->GetTimerManager().SetTimer(WrongMessageTimerHandle, FTimerDelegate::CreateLambda([this]()
	{
		BigPainterRoleTextBlock->SetVisibility(ESlateVisibility::Hidden);
	}), 3, false);
}

void UCatchMindWidget::ShowParticipant()
{
	PainterRoleTextBlock->SetVisibility(ESlateVisibility::Hidden);
	ParticipantRoleTextBlock->SetVisibility(ESlateVisibility::Visible);
	BigPainterRoleTextBlock->SetVisibility(ESlateVisibility::Hidden);
	BigParticipantRoleTextBlock->SetVisibility(ESlateVisibility::Visible);
	SuggestionTitleTextBlock->SetVisibility(ESlateVisibility::Visible);
	SuggestionTextBlock->SetVisibility(ESlateVisibility::Visible);
	HintTitleTextBlock->SetVisibility(ESlateVisibility::Visible);
	HintTextBlock->SetVisibility(ESlateVisibility::Visible);
	WrongTextBlock->SetVisibility(ESlateVisibility::Hidden);
	CorrectTextBlock->SetVisibility(ESlateVisibility::Hidden);
	AnswerTextBox->SetVisibility(ESlateVisibility::Visible);
	AnswerTextBox->SetIsEnabled(true);
	TimeUpTextBlock->SetVisibility(ESlateVisibility::Hidden);

	GetWorld()->GetTimerManager().SetTimer(WrongMessageTimerHandle, FTimerDelegate::CreateLambda([this]()
	{
		BigParticipantRoleTextBlock->SetVisibility(ESlateVisibility::Hidden);
	}), 3, false);
}

void UCatchMindWidget::ShowSomeoneWin(const FString& SomeoneName) const
{
	HideAll();
	
	AnswerTextBox->SetIsEnabled(false);
	CorrectTextBlock->SetVisibility(ESlateVisibility::Visible);
	CorrectTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%s님이 정답을 맞추셨습니다!"), *SomeoneName)));
}

void UCatchMindWidget::ShowWin(const FString& MyName) const
{
	HideAll();
	
	AnswerTextBox->SetIsEnabled(false);
	CorrectTextBlock->SetVisibility(ESlateVisibility::Visible);
	CorrectTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%s님 정답입니다!"), *MyName)));
}

void UCatchMindWidget::ShowWrong()
{
	if (WrongMessageTimerHandle.IsValid())
	{
		WrongMessageTimerHandle.Invalidate();
	}

	WrongTextBlock->SetVisibility(ESlateVisibility::Visible);
	AnswerTextBox->SetText(FText());
	
	GetWorld()->GetTimerManager().SetTimer(WrongMessageTimerHandle, FTimerDelegate::CreateLambda([this]()
	{
		WrongTextBlock->SetVisibility(ESlateVisibility::Hidden);
	}), 2, false);
}

void UCatchMindWidget::SetSuggestionText(const FString& Suggestion, const FString& Hint) const
{
	SuggestionTextBlock->SetText(FText::FromString(Suggestion));
	HintTextBlock->SetText(FText::FromString(Hint));
}

void UCatchMindWidget::ShowTimeUp() const
{
	PainterRoleTextBlock->SetVisibility(ESlateVisibility::Hidden);
	ParticipantRoleTextBlock->SetVisibility(ESlateVisibility::Hidden);
	BigPainterRoleTextBlock->SetVisibility(ESlateVisibility::Hidden);
	BigParticipantRoleTextBlock->SetVisibility(ESlateVisibility::Hidden);
	SuggestionTitleTextBlock->SetVisibility(ESlateVisibility::Hidden);
	SuggestionTextBlock->SetVisibility(ESlateVisibility::Hidden);
	HintTitleTextBlock->SetVisibility(ESlateVisibility::Hidden);
	HintTextBlock->SetVisibility(ESlateVisibility::Hidden);
	WrongTextBlock->SetVisibility(ESlateVisibility::Hidden);
	CorrectTextBlock->SetVisibility(ESlateVisibility::Hidden);
	AnswerTextBox->SetVisibility(ESlateVisibility::Hidden);
	AnswerTextBox->SetIsEnabled(false);
	TimeUpTextBlock->SetVisibility(ESlateVisibility::Visible);
}

void UCatchMindWidget::OnAnswerTextCommitted(const FText& InText, ETextCommit::Type InCommitMethod)
{
	if (InCommitMethod == ETextCommit::Type::OnEnter)
	{
		//PC를 통해서 서버에 정답 제출
		if (const auto PCPaint = Cast<ASQP_PC_PaintRoom>(GetWorld()->GetFirstPlayerController()))
		{
			PCPaint->Server_ReceiveCatchMindAnswer(InText.ToString());
		}

		//비움
		AnswerTextBox->SetText(FText());
	}
}