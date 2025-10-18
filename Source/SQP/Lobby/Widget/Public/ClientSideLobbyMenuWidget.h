// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LobbyMenuWidgetBase.h"
#include "ClientSideLobbyMenuWidget.generated.h"

class UToggleButton;
class UTextBlock;

UCLASS()
class SQP_API UClientSideLobbyMenuWidget : public ULobbyMenuWidgetBase
{
	GENERATED_BODY()

public:
	UClientSideLobbyMenuWidget();
	
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UToggleButton> ReadyButton;

	virtual void OnOtherPlayerEnter(FPlayerInfo& NewPlayerInfo) override;

protected:
	UFUNCTION()
	void OnReadyButtonClicked(bool Value);
};
