// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SQPGameState.generated.h"

struct FPlayerInfo;

UCLASS()
class SQP_API ASQPGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PaintRenderTarget(FVector Start, FVector End, uint8 BrushIndex, float BrushSize);

#pragma region 로비 관련

	UFUNCTION()
	void OnNewPlayerLogin(APlayerController* NewPlayer);

	UFUNCTION()
	void OnOldPlayerLogout(APlayerController* OldPlayer);

	UPROPERTY(ReplicatedUsing = OnRep_ExistingPlayerInfoArray)
	TArray<FPlayerInfo> ExistingPlayerInfoArray;

	UFUNCTION()
	void OnRep_ExistingPlayerInfoArray();

#pragma endregion
	
};