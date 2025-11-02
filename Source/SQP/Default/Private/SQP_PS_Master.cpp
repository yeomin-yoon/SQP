// Fill out your copyright notice in the Description page of Project Settings.


#include "SQP_PS_Master.h"

#include "PlaygroundScoreWidget.h"
#include "SQP.h"
#include "SQP_PC_PaintRoom.h"
#include "SQP_PS_LobbyRoomComponent.h"
#include "SQP_PS_PaintRoomComponent.h"
#include "Net/UnrealNetwork.h"

ASQP_PS_Master::ASQP_PS_Master()
{
	//로비 관련 컴포넌트 추가
	LobbyRoom = CreateDefaultSubobject<USQP_PS_LobbyRoomComponent>(FName("LobbyRoomComponent"));
	LobbyRoom->SetIsReplicated(true);

	//페인트 룸 관련 컴포넌트 추가
	PaintRoom = CreateDefaultSubobject<USQP_PS_PaintRoomComponent>(FName("PaintRoomComponent"));
	PaintRoom->SetIsReplicated(true);
}

void ASQP_PS_Master::BeginPlay()
{
	Super::BeginPlay();

	PRINTLOGNET(TEXT("%s"), *GetPlayerName());
}

void ASQP_PS_Master::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (GetLocalRole() == ROLE_Authority)
	{
		PRINTLOGNET(TEXT("PS_Master::EndPlay()"));
	}
	else
	{
		PRINTLOGNET(TEXT("PS_Master::EndPlay()"));
	}
}

void ASQP_PS_Master::Destroyed()
{
	Super::Destroyed();

	if (HasAuthority())
	{
		PRINTLOGNET(TEXT("PS_Master:Destroyed()"));
	}
}

void ASQP_PS_Master::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);

	PRINTLOGNET(TEXT("Copy Properties!"));
}

void ASQP_PS_Master::OnRep_Score()
{
	Super::OnRep_Score();

	Cast<ASQP_PC_PaintRoom>(GetWorld()->GetFirstPlayerController())->PlaygroundScoreWidget->UpdatePlaygroundScore();
}

void ASQP_PS_Master::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//컴포넌트 리플리케이션 활성화
	DOREPLIFETIME(ASQP_PS_Master, LobbyRoom);
	DOREPLIFETIME(ASQP_PS_Master, PaintRoom);
}
