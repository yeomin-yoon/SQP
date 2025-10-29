// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaintGameActor.generated.h"

UCLASS()
class SQP_API APaintGameActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APaintGameActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY()
	TObjectPtr<class UIMGManager> IMGManager;
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynMat;
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ShowRandomImage(UTexture2D* Image);
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void StartGameTimer();
	void CountDown();
	void CountDownText();
	FTimerHandle StartTimer;
	int32 StartTime;
	void StartGameTimerFinished();
	
};
