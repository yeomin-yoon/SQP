// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LobbyMenuWidgetBase.h"
#include "HostSideLobbyMenuWidget.generated.h"

class UActiveButton;
class UTextBlock;

UCLASS()
class SQP_API UHostSideLobbyMenuWidget : public ULobbyMenuWidgetBase
{
	GENERATED_BODY()

public:
	UHostSideLobbyMenuWidget();

	virtual void NativeConstruct() override;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UActiveButton> StartButton;
	
	virtual void OnOtherPlayerEnter(FPlayerInfo& NewPlayerInfo) override;

	UFUNCTION()
	void OnStartButtonClicked();
};