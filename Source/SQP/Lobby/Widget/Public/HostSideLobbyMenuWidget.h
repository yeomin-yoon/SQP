// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LobbyMenuWidgetBase.h"
#include "HostSideLobbyMenuWidget.generated.h"

class UScrollBox;
class UActiveButton;
class UTextBlock;

UCLASS()
class SQP_API UHostSideLobbyMenuWidget : public ULobbyMenuWidgetBase
{
	GENERATED_BODY()

public:
	UHostSideLobbyMenuWidget();

	virtual void NativeConstruct() override;

	virtual void OnOtherPlayerEnter(FPlayerInfo& NewPlayerInfo) override;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UActiveButton> StartButton;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UScrollBox> SaveDataScrollBox;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> CancelSaveDataSelectionButton;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> PaintRoomSaveInfoWidgetClass;
	
	UFUNCTION()
	void OnStartButtonClicked();
};