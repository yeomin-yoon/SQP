// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SavablePaint.h"
#include "GameFramework/Actor.h"
#include "SQP_PaintableActor.generated.h"

UCLASS()
class SQP_API ASQP_PaintableActor : public AActor, public ISavablePaint
{
	GENERATED_BODY()

public:
	ASQP_PaintableActor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGuid PersistantActorID;

	virtual FGuid GetPersistantActorID() override;
};
