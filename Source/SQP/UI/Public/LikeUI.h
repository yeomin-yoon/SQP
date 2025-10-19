// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LikeUI.generated.h"

/**
 * 
 */
UCLASS()
class SQP_API ULikeUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class URichTextBlock> LikeText;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class URichTextBlock> LikeNumberText;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UButton> LikeBtn;

	int32 LikeNum = 0;
	void CountLikes();
	UFUNCTION()
	void OnLike();
};
