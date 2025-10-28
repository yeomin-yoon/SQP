// Fill out your copyright notice in the Description page of Project Settings.

#include "SQP_PaintableActor.h"

#include "SQP.h"

ASQP_PaintableActor::ASQP_PaintableActor()
{
	//액터 틱 활성화
	PrimaryActorTick.bCanEverTick = true;
}

void ASQP_PaintableActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (PersistantActorID.IsValid())
	{
		PRINTLOG(TEXT("%s is already Assigned Persistant Actor ID!"), *GetActorNameOrLabel());
	}
	else
	{
		//아직 고유 ID가 없다면 새롭게 할당
		PersistantActorID = FGuid::NewGuid();
		PRINTLOG(TEXT("%s Has been Assigned New Persistant Actor ID!"), *GetActorNameOrLabel());
	}
}

FGuid ASQP_PaintableActor::GetPersistantActorID()
{
	return PersistantActorID;
}

void ASQP_PaintableActor::ValidPersistantActorID()
{
	if (PersistantActorID.IsValid())
	{
		PRINTLOG(TEXT("%s is already Assigned Persistant Actor ID!"), *GetActorNameOrLabel());
	}
	else
	{
		//아직 고유 ID가 없다면 새롭게 할당
		PersistantActorID = FGuid::NewGuid();
		PRINTLOG(TEXT("%s Has been Assigned New Persistant Actor ID!"), *GetActorNameOrLabel());
	}
}