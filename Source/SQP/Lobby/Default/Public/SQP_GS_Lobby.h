// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SQPGameState.h"
#include "SQP_GS_Lobby.generated.h"

UCLASS()
class SQP_API ASQP_GS_Lobby : public ASQPGameState
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION()
	void OnNewPlayerLogin(APlayerController* LoginPlayer);

	UFUNCTION()
	void OnOldPlayerLogout(const AController* LogoutPlayer);

	UPROPERTY(ReplicatedUsing = OnRep_ExistingPlayerInfoArray)
	TArray<FPlayerInfo> ExistingPlayerInfoArray;

	UFUNCTION()
	void OnRep_ExistingPlayerInfoArray();
};
