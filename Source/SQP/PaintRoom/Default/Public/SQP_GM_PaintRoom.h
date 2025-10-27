// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SQP_GM_PaintRoom.generated.h"

UCLASS()
class SQP_API ASQP_GM_PaintRoom : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASQP_GM_PaintRoom();
	
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> PaintRoomWidgetClass;
};