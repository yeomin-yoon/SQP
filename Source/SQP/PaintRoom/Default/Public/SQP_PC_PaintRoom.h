// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SQPPlayerController.h"
#include "SQP_PC_PaintRoom.generated.h"

UCLASS()
class SQP_API ASQP_PC_PaintRoom : public ASQPPlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(Server, Reliable)
	void Server_PaintColorChange(FLinearColor Value);
};