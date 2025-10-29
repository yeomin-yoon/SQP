// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ReadyActor.generated.h"

DECLARE_DELEGATE(FOnTimerFinished)

UCLASS()
class SQP_API AReadyActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AReadyActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(Replicated)
	TArray<TObjectPtr<class ASQP_PS_PaintRoom>> ReadyPlayerState;
	
	bool bReady = false;
	void StartReadyTimer();
	void CountDown();
	void CountDownText();
	FTimerHandle ReadyTimer;
	int32 ReadyTime;

	FOnTimerFinished OnTimerFinished;
};
