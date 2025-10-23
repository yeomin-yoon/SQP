// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SessionInfoWidget.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class SQP_API USessionInfoWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable)
	void SetBindingSessionIdx(const int32 Value) { BindingSessionIdx = Value; }

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UTextBlock> RoomNameTextBlock;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> JoinButton;

	UFUNCTION()
	void OnJoinButtonClicked();

private:
	UPROPERTY(VisibleAnywhere)
	int32 BindingSessionIdx;

};