// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SQP_PS_PaintRoom.generated.h"

/**
 * 
 */
UCLASS()
class SQP_API ASQP_PS_PaintRoom : public APlayerState
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FLinearColor SelectedColor;
};
