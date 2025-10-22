// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SQPGameState.h"
#include "SQP_GS_Lobby.generated.h"

class ASQP_PC_Lobby;
class ASQP_GM_Lobby;

UCLASS()
class SQP_API ASQP_GS_Lobby : public ASQPGameState
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void AddPlayerState(APlayerState* PlayerState) override;
	
	virtual void RemovePlayerState(APlayerState* PlayerState) override;

	UFUNCTION()
	ASQP_PC_Lobby* GetHostPlayerController();
	
	UFUNCTION()
	ASQP_GM_Lobby* GetHostGameMode();
	
	UFUNCTION()
	void OnNewPlayerLogin(APlayerController* LoginPlayer);

	UFUNCTION()
	void OnOldPlayerLogout(const AController* LogoutPlayer);

	UPROPERTY(ReplicatedUsing = OnRep_ExistingPlayerInfoArray)
	TArray<FPlayerInfo> ExistingPlayerInfoArray;

	UFUNCTION()
	void OnRep_ExistingPlayerInfoArray();
};
