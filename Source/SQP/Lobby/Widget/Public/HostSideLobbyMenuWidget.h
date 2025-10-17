// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LobbyMenuWidgetBase.h"
#include "HostSideLobbyMenuWidget.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class SQP_API UHostSideLobbyMenuWidget : public ULobbyMenuWidgetBase
{
	GENERATED_BODY()

public:
	UHostSideLobbyMenuWidget();
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> StartButton;
	
	virtual void OnOtherPlayerEnter(FPlayerInfo& NewPlayerInfo) override;

	virtual void OnOtherPlayerExit(FPlayerInfo& OldPlayerInfo) override;
};