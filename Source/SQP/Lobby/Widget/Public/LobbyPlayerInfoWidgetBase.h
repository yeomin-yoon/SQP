// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyPlayerInfoWidgetBase.generated.h"

class UTextBlock;

UCLASS()
class SQP_API ULobbyPlayerInfoWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> PrefixTextBlock;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> RoleTextBlock;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> NameTextBlock;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> SuffixTextBlock;

	UPROPERTY()
	FString BindingPlayerUniqueId;
};