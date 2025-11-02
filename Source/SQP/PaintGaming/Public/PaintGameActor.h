// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaintGameActor.generated.h"

DECLARE_DELEGATE(FOnTimerFinished)

UCLASS()
class SQP_API APaintGameActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APaintGameActor();

	FOnTimerFinished OnTimerFinished;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override; 
	
	UPROPERTY()
	TObjectPtr<class ASQP_GS_PaintRoom> GS;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ShowRandomImage(UTexture2D* Image);

	UPROPERTY(Replicated)
	float ElapsedRot = 0.f;
	UPROPERTY()
	TObjectPtr<class UMaterialInstanceDynamic> DynMat;
	
	void CountDown();
	void CountDownText();
	FTimerHandle StartTimer;
	int32 StartTime;
	void StartGameTimerFinished();
};
