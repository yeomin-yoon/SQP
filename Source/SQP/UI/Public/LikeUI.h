// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LikeUI.generated.h"


UCLASS()
class SQP_API ULikeUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class URichTextBlock> LikeNumberText;
	
protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class URichTextBlock> LikeText;

public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UButton> LikeBtn;
	
	UPROPERTY()
	TSet<TWeakObjectPtr<APlayerController>> LikedPlayers;
	
	template <typename T>
	void CleanWeakSet(TSet<TWeakObjectPtr<T>>& Set)
	{
		for (auto It = Set.CreateIterator(); It; ++It)
		{
			if (!It->IsValid())
			{
				It.RemoveCurrent();
			}
		}
	}
};
