// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SQPLobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SQP_API ASQPLobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASQPLobbyGameMode();
	
	virtual void PostLogin(APlayerController* NewPlayer) override;

	UFUNCTION()
	void OnPlayerReadyStateChanged();

	UFUNCTION()
	void CheckAllPlayersReady();
};
