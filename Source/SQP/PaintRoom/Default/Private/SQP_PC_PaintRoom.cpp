// Fill out your copyright notice in the Description page of Project Settings.


#include "PaintRoom/Default/Public/SQP_PC_PaintRoom.h"

#include "SQP_PS_PaintRoom.h"

void ASQP_PC_PaintRoom::Server_PaintColorChange_Implementation(const FLinearColor Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, TEXT("PaintColorChange!"));
	GetPlayerState<ASQP_PS_PaintRoom>()->SelectedColor = Value;
}