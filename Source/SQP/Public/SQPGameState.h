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
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PaintRenderTarget(FVector Start, FVector End, uint8 BrushIndex, float BrushSize);
};