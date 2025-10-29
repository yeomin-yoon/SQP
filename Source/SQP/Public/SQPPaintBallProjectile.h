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

	UFUNCTION()
	FORCEINLINE void SetPaintColor(const FLinearColor& Value);
	UFUNCTION()
	FORCEINLINE void SetBrushSize(const float& Value) { BrushSize = Value; }
	UFUNCTION()
	FORCEINLINE void SetPaintBallOwner(class ASQP_PS_PaintRoom* Value) { OwnerPlayerState = Value; } 

protected:
	UPROPERTY(EditDefaultsOnly)
	bool bIsReal;

	UPROPERTY()
	FLinearColor PaintColor;

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynMat;
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ColorPaintBall(FLinearColor Color);

	UPROPERTY()
	float BrushSize;

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_TryPaint(const FLinearColor BrushColor, const float BrushSizeValue);

	UPROPERTY()
	TObjectPtr<class ASQP_PS_PaintRoom> OwnerPlayerState;
	UFUNCTION(Server, Reliable)
	void Server_AddReady(class AReadyActor* ReadyActor);
};