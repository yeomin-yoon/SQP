// Fill out your copyright notice in the Description page of Project Settings.


#include "SQP_GS_PaintRoom.h"

#include "SQPPaintWorldSubsystem.h"
#include "SQP_SG_PaintRoom.h"
#include "Net/UnrealNetwork.h"

ASQP_GS_PaintRoom::ASQP_GS_PaintRoom()
{
	//리플리케이션 활성화
	bReplicates = true;
}

void ASQP_GS_PaintRoom::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//리플리케이션 등록
	DOREPLIFETIME(ASQP_GS_PaintRoom, PaintExecutionDataSnapshot);
}

void ASQP_GS_PaintRoom::OnRep_PaintExecutionDataSnapshot()
{
	//세이브 데이터를 받는 건 클라이언트만 하면 되는 작업
	if (HasAuthority())
	{
		return;
	}
	
	//최초 리플리케이션 이후에는 적용 거부
	if (bHasInitialPaintDataBeenApplied)
	{
		return;
	}
	
	//스냅샷 이후에는 리플리케이션 비활성화
	bHasInitialPaintDataBeenApplied = true;

	//전달받은 데이터로 PEDContainer를 재구축
	TMap<FGuid, FPaintExecutionDataWrapper> PEDContainer;
	for (auto& PED : PaintExecutionDataSnapshot)
	{
		PEDContainer.FindOrAdd(PED.UniqueID).PEDArray.Emplace(PED);
	}

	//재구축한 데이터를 전달하여 그릴 수 있도록
	if (const auto Subsystem = GetWorld()->GetSubsystem<USQPPaintWorldSubsystem>())
	{
		Subsystem->LoadPaintOfWorld(PEDContainer);
	}
}