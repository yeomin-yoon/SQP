// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SQP_PS_Lobby.h"
#include "Blueprint/UserWidget.h"
#include "LobbyMenuWidgetBase.generated.h"

class UButton;
struct FPlayerInfo;
class UVerticalBox;
class UTextBlock;

UCLASS()
class SQP_API ULobbyMenuWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	FTimerHandle UpdateNumOfPlayerTimerHandle;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> NumOfPlayers;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UVerticalBox> PlayerInfoBox;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> LeaveLobbyButton;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> PlayerInfoWidgetClass;
	
	UFUNCTION()
	virtual void OnOtherPlayerEnter(FPlayerInfo& NewPlayerInfo);

	UFUNCTION()
	virtual void OnOtherPlayerExit(FPlayerInfo& OldPlayerInfo);

	UFUNCTION()
	virtual void OnPostLogin(const TArray<FPlayerInfo>& ExistingPlayerInfoArray);

	UFUNCTION()
	virtual void OnLeaveButtonClicked();

	UFUNCTION()
	virtual void UpdatePlayerLobbyState(const FString& PlayerUniqueId, const ELobbyState& Value);
	
	UPROPERTY()
	TMap<FString, UUserWidget*> UniqueIdToWidgetMap;
};
