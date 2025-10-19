// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SwimComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SQP_API USwimComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USwimComponent();

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<ACharacter> OwnerCharacter;

	float ElapsedTime = 0.f;
	float SpeedUpTime = 1.5f;

	UFUNCTION(Server, Reliable)
	void Server_ChangeMaxSpeed(float Speed);

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY()
	TMap<UPrimitiveComponent*, UTextureRenderTarget2D*> PaintRTCache;
	UFUNCTION(Server, Reliable)
	void Server_CheckOnPaint();
	bool IsOnPaint();
};
