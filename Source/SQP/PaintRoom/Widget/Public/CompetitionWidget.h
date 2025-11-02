// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CompetitionWidget.generated.h"

class UTextBlock;

UCLASS()
class SQP_API UCompetitionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeDestruct() override;
	
	UFUNCTION()
	void ShowSomeoneWin(const FString& SomeoneName);

	UFUNCTION()
	void ShowWin(const FString& MyName);

	UFUNCTION()
	void HideAll() const;

protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> CorrectTextBlock;

	FTimerHandle TimerHandle;
};
