// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CatchMindWidget.generated.h"

class UEditableTextBox;
class UTextBlock;

UCLASS()
class SQP_API UCatchMindWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void HideAll() const;

	UFUNCTION()
	void ShowPainter() const;

	UFUNCTION()
	void ShowParticipant() const;

	UFUNCTION()
	void ShowSomeoneWin(const FString& SomeoneName);

	UFUNCTION()
	void ShowWin(const FString& MyName);

	UFUNCTION()
	void ShowWrong();

	UFUNCTION()
	void SetSuggestionText(const FString& Suggestion, const FString& Hint) const;

protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> PainterRoleTextBlock;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> ParticipantRoleTextBlock;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> SuggestionTitleTextBlock;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> SuggestionTextBlock;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> HintTextBlock;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> HintTitleTextBlock;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UEditableTextBox> AnswerTextBox;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> WrongTextBlock;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> CorrectTextBlock;

	UFUNCTION()
	void OnAnswerTextCommitted(const FText& InText, ETextCommit::Type InCommitMethod);

	FTimerHandle WrongMessageTimerHandle;
};