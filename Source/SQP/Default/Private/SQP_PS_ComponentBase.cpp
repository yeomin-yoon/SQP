// Fill out your copyright notice in the Description page of Project Settings.

#include "SQP_PS_ComponentBase.h"

#include "SQP_GI.h"
#include "SQP_PC_Lobby.h"
#include "SQP_PS_Master.h"

USQP_GI* USQP_PS_ComponentBase::GetSQPGameInstance() const
{
	return Cast<USQP_GI>(GetWorld()->GetGameInstance());
}

ASQP_PC_Lobby* USQP_PS_ComponentBase::GetWorldFirstPCMaster() const
{
	return Cast<ASQP_PC_Lobby>(GetWorld()->GetFirstPlayerController());
}

ASQP_PS_Master* USQP_PS_ComponentBase::GetBindingPSMaster() const
{
	return Cast<ASQP_PS_Master>(GetOwner());
}

void USQP_PS_ComponentBase::Server_NotifyUserInfo_Implementation(FUserInfo UserInfo)
{
	
}