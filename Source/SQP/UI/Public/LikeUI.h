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
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class URichTextBlock> LikeText;
	UPROPERTY(meta=(BindWidget), Replicated)
	TObjectPtr<class URichTextBlock> LikeNumberText;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UButton> LikeBtn;
	
	UPROPERTY()
	TSet<TWeakObjectPtr<AActor>> LikePlayers;
	UPROPERTY(ReplicatedUsing = OnRep_UpdateLikes)
	int32 LikeNum = 0;

	UFUNCTION()
	void OnRep_UpdateLikes(int32 Num);
	UFUNCTION()
	void OnClick();
	UFUNCTION(Server, Reliable)
	void Server_OnLike(AActor* Actor);
	
public:
	UPROPERTY()
	TObjectPtr<AActor> ClickingActor;
};
