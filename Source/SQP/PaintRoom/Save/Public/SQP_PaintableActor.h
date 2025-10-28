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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FGuid PersistantActorID;

	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION(BlueprintCallable, Category = "Savable Paint")
	virtual FGuid GetPersistantActorID() override;

	UFUNCTION(BlueprintCallable, Category = "Savable Paint")
	virtual void ValidPersistantActorID() override;
};
