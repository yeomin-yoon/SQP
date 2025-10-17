// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SQPPlayerState.generated.h"

struct FPlayerInfo;

UCLASS()
class SQP_API ASQPPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ASQPPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void SetReadyState(bool Value);

	bool GetIsReady() const { return bIsReady; }

	UFUNCTION()
	FPlayerInfo GetPlayerInfo();

protected:
	UPROPERTY(ReplicatedUsing = OnRep_bIsReady)
	bool bIsReady;

	UFUNCTION()
	void OnRep_bIsReady();
};
