// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SQP_PS_ComponentBase.h"
#include "Components/ActorComponent.h"
#include "SQP_PS_PaintRoomComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SQP_API USQP_PS_PaintRoomComponent : public USQP_PS_ComponentBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY()
	FLinearColor SelectedColor;

	UPROPERTY()
	float SelectedBrushSize;

	UFUNCTION()
	void IncreaseLikeCounter();

	UPROPERTY(ReplicatedUsing=OnRep_LikeCounter)
	int32 LikeCounter = 0;
	
	UFUNCTION()
	void OnRep_LikeCounter() const;
};
