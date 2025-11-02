// Fill out your copyright notice in the Description page of Project Settings.

#include "SQP_PaintableActor.h"

#include "CompetitorName.h"
#include "SQPPaintWorldSubsystem.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"


ASQP_PaintableActor::ASQP_PaintableActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//아직 고유 ID가 없다면 새롭게 할당
	if (PersistantActorID.IsValid() == false)
	{
		PersistantActorID = FGuid::NewGuid();
	}

	//리플리케이션 대상
	bReplicates = true;
}

void ASQP_PaintableActor::BeginPlay()
{
	Super::BeginPlay();

	
}

void ASQP_PaintableActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// PRINTLOGNET(TEXT("OnConstruction!!!"));
	//
	// if (const auto Subsystem = GetWorld()->GetSubsystem<USQPPaintWorldSubsystem>())
	// {
	// 	if (const auto MeshComp = GetComponentByClass<UStaticMeshComponent>())
	// 	{
	// 		MeshComp->SetMaterial(0, Subsystem->GetCanvasMaterialBase());
	// 	}
	// 	
	// 	Subsystem->ResetCanvasMaterialUV(this);
	// }
}

void ASQP_PaintableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

FGuid ASQP_PaintableActor::GetPersistantActorID()
{
	return PersistantActorID;
}

void ASQP_PaintableActor::ClearPaint()
{
	if (const auto Subsystem = GetWorld()->GetSubsystem<USQPPaintWorldSubsystem>())
	{
		Subsystem->ClearPaint(this);
	}
}

void ASQP_PaintableActor::Multicast_ClearPaint_Implementation()
{
	if (const auto Subsystem = GetWorld()->GetSubsystem<USQPPaintWorldSubsystem>())
	{
		Subsystem->ClearPaint(this);
	}
}

void ASQP_PaintableActor::ResetCanvasMaterialUV()
{
	if (const auto Subsystem = GetWorld()->GetSubsystem<USQPPaintWorldSubsystem>())
	{
		Subsystem->ResetCanvasMaterialUV(this);
	}
}
