// Fill out your copyright notice in the Description page of Project Settings.

#include "SQP_PaintableActor.h"

#include "HeadMountedDisplayTypes.h"
#include "SQP.h"
#include "SQPPaintWorldSubsystem.h"
#include "Kismet/KismetRenderingLibrary.h"

ASQP_PaintableActor::ASQP_PaintableActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//아직 고유 ID가 없다면 새롭게 할당
	if (PersistantActorID.IsValid() == false)
	{
		PersistantActorID = FGuid::NewGuid();
	}
}

void ASQP_PaintableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASQP_PaintableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FGuid ASQP_PaintableActor::GetPersistantActorID()
{
	return PersistantActorID;
}