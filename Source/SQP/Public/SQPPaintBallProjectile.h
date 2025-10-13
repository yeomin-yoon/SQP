// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileBase.h"
#include "SQPPaintBallProjectile.generated.h"

UCLASS()
class SQP_API ASQPPaintBallProjectile : public AProjectileBase
{
	GENERATED_BODY()

public:
	ASQPPaintBallProjectile();

	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnOverlapBeginCallback (UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_TryPaint();

protected:
	UPROPERTY(EditDefaultsOnly)
	bool bIsReal;
};